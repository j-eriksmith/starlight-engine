#include "PoolAllocTestFixture.h"
#include <chrono>
#include <random>
#include <cassert>
#include "Debug.h"

void PoolAllocTestFixture::FillPool(Pool& p, std::vector<uint8_t*>& addrs)
{
	uint8_t* curPtr = reinterpret_cast<uint8_t*>(p.Alloc());
	while (curPtr)
	{
		addrs.push_back(curPtr);
		curPtr = reinterpret_cast<uint8_t*>(p.Alloc());
	}
	assert(!p.freeBlocks);
	assert(addrs.size() == p.totalBlocks);
}

void PoolAllocTestFixture::FreePool(Pool& p, std::vector<uint8_t*>& addrs)
{
	while (addrs.size())
	{
		p.Free(addrs[addrs.size() - 1]);
		addrs.pop_back();
	}
	assert(p.freeBlocks == p.totalBlocks);
}

void PoolAllocTestFixture::RunTests()
{
	// TestDefaultConstruction();
	TestAllocFree();
	TestOverflowAllocFree();
	TestFindBestFit();
	TestGetClosestPowerOfTwo();
}

void PoolAllocTestFixture::TestAllocFree()
{
	Log("PoolAllocTestFixture::TestAlloc");
	PoolAllocator p(100000);
	// std::cout << "sizeof(uint) " << sizeof(uint) << std::endl;
	for (int i = 4; i <= p.largestBlockSize; i <<= 1)
	{
		Log("PoolAllocTestFixture::TestAlloc -- Testing pool of size " << i);
		TestPool(p, i);
	}
	Log("PoolAllocTestFixture::TestAlloc -- exiting");
}

void PoolAllocTestFixture::TestDefaultConstruction()
{
	Log("PoolAllocTestFixture::TestDefaultConstruction");
	PoolAllocator p(100000);
	Log("PoolAllocTestFixture::TestDefaultConstruction -- exiting");
}

void PoolAllocTestFixture::TestFindBestFit(int reps)
{
	uint seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 random(seed);
	for (int i = 4, j = 2; i < 1024; j = i, i <<= 1)
	{
		for (int rep = 0; rep < reps; ++rep)
		{
			// Sets the lower bound to (the previous power of 2) + 1 and the upper bound to
			// the current power of 2. The best fit should always be the current power of 2.
			std::uniform_int_distribution<int> distribution(j + 1, i);
			assert(PoolAllocator::FindBestFit(distribution(random), 1024) == i);
		}

	}
}

void PoolAllocTestFixture::TestGetClosestPowerOfTwo(int reps)
{
	uint seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 random(seed);
	for (int i = 4, j = 2; i < 1024; j = i, i <<= 1)
	{
		for (int rep = 0; rep < reps; ++rep)
		{
			// Sets the lower bound to (the previous power of 2) + 1 and the upper bound to
			// the current power of 2. The best fit should always be the current power of 2.
			std::uniform_int_distribution<int> distribution(j, i - 1);
			assert(PoolAllocator::GetClosestPowerOfTwo(distribution(random)) == j);
		}

	}
}

void PoolAllocTestFixture::TestOverflowAllocFree()
{
	PoolAllocator p(10000);
	for (int i = 4; i < p.largestBlockSize; i <<= 1)
	{
		Pool& curPool = p.poolAddresses[i];
		// Allocate blocks in the current pool until it is full, then perform one more allocation.
		// This final allocation should be assigned to the pool of size i << 1, or i * 2.
		std::vector<uint8_t*> addrs;
		FillPool(curPool, addrs);
		uint8_t* curPtr = reinterpret_cast<uint8_t*>(p.Alloc(i));
		// Since our pool that contains blocks of size i is full, 
		// our PoolAllocator should direct our alloc request to the
		// next free pool, which contains blocks of size i * 2.
		assert(p.poolAddresses[i << 1].IsValidAddress(curPtr));
		p.Free(i, curPtr);
		assert(!p.poolAddresses[i].freeBlocks);
		assert(p.poolAddresses[i << 1].freeBlocks == p.poolAddresses[i << 1].totalBlocks);
		FreePool(curPool, addrs);
	}
}

void PoolAllocTestFixture::TestPool(PoolAllocator& p, uint sz)
{
	Pool& curPool = p.poolAddresses[sz];
	p.poolAddresses[sz].LogPoolInfo();
	std::vector<uint8_t*> st(curPool.totalBlocks);
	// Alloc until filled
	// Things to check for at each iteration:
	// 1. freeBlocks >= 0 && freeBlocks <= totalBlocks
	// 2. Initialized blocks == totalBlocks after full allocation
	// 3. head is always in between start and start + regionSize
	for (int i = 0; i < curPool.totalBlocks; ++i)
	{
		st[i] = reinterpret_cast<uint8_t*>(p.Alloc(sz));
		assert(curPool.head >= curPool.start && curPool.head <= curPool.start + curPool.regionSize);
		assert(!(reinterpret_cast<uintptr_t>(curPool.head) % curPool.blockSize));
	}
	assert(curPool.initializedBlocks == curPool.totalBlocks);
	assert(curPool.freeBlocks == 0);
	assert(curPool.head == curPool.start + curPool.regionSize);

	unsigned int numAddrs = st.size();
	int totalFree = 0;
	for (int i = 0; i < p.poolAddresses[sz].totalBlocks; ++i)
	{
		assert(curPool.freeBlocks == totalFree++);
		p.Free(sz, st[numAddrs - 1 - i]);
		assert(curPool.head >= curPool.start && curPool.head <= curPool.start + curPool.regionSize);
		assert(!(reinterpret_cast<uintptr_t>(curPool.head) % curPool.blockSize));
		st.pop_back();
	}
	assert(curPool.freeBlocks == curPool.totalBlocks);
	assert(curPool.head == curPool.start);
	// randomly alloc and free blocks of memory from pool
	uint seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 random(seed);
	std::uniform_int_distribution<int> distribution(0, 5);
	Log("PoolAllocTestFixture::testPool -- starting randomized Pool stress test.");
	for (int i = 0; i < sz * 2; ++i)
	{
		if (distribution(random) < 2)
		{
			Log("PoolAllocTestFixture::testPool -- Allocating block");
			uint8_t* ptr = reinterpret_cast<uint8_t*>(p.Alloc(sz));
			if (ptr)
			{
				st.push_back(ptr);
			}
		}
		else
		{
			if (st.size())
			{
				Log("PoolAllocTestFixture::testPool -- Freeing Block");
				p.Free(sz, st[st.size() - 1]);
				st.pop_back();
			}
		}
		assert(curPool.head >= curPool.start && curPool.head <= curPool.start + curPool.regionSize);
		assert(!(reinterpret_cast<uintptr_t>(curPool.head) % curPool.blockSize));
	}
}