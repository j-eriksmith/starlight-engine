#include "PoolAllocTestFixture.h"
#include <chrono>
#include <random>

// ============================== TESTS ==============================
class payToWinClub;
class Bag;
class Item;
class Player;
class MtxPlayer;

void testPoolAddrs(uint start)
{
	// 50 Mib storage 52428800
	PoolAllocator p = PoolAllocator(102400);
	for (uint i = start; i >= 4; i /= 2)
	{
		//std::cout << "Address of pool for size " << i << " blocks is "
		//	<< m.getPoolAddress(i) << std::endl;
	}
}

void initPoolAllocator()
{
	// Alloc ~50 MiB of storage
	PoolAllocator p = PoolAllocator(102400);
}

//struct four
//{
//	uint data;
//};
//
//// 32
//// size 24 object
//struct twenty_four
//{
//	four data[6];
//};
//
//struct sixty_four
//{
//	twenty_four data[2];
//	four data[4];
//};
//
//struct one_twenty_eight
//{
//	sixty_four data[2];
//};

// test 128
// size 88
class Item
{
	double price;
	uint metaData[10];
};

// 256
// size 196
class Bag
{
	Item bag[2];

public:
	explicit Bag() {}
};

void deletePlayer(uint itemSize, Player* p, PoolAllocator& m)
{
	m.Free(itemSize, p);
}

// @todo 4, 8, 16, 64

void PoolAllocTestFixture::testPool(PoolAllocator& p, uint sz)
{
	//Pool curPool = p.poolAddresses[sz];
	p.poolAddresses[sz].LogPoolInfo();
	std::vector<uint8_t*> st(p.poolAddresses[sz].totalBlocks);
	// Alloc until filled
	for (int i = 0; i < p.poolAddresses[sz].totalBlocks; ++i)
	{
		p.poolAddresses[sz].LogBlockInfo();
		st[i] = reinterpret_cast<uint8_t*>(p.Alloc(sz));
	}
	std::cout << "size of addr vec: " << st.size() << std::endl;
	int numAddrs = st.size();
	for (int i = 0; i < p.poolAddresses[sz].totalBlocks; ++i)
	{
		p.poolAddresses[sz].LogBlockInfo();
		p.Free(sz, st[numAddrs - 1 - i]);
		st.pop_back();
	}
	// randomly alloc and free blocks of memory from pool
	uint seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 random(seed);
	std::uniform_int_distribution<int> distribution(0, 1);
	for (int i = 0; i < sz * 2; ++i)
	{
		if (!distribution(random))
		{
			uint8_t* ptr = reinterpret_cast<uint8_t*>(p.Alloc(sz));
			p.poolAddresses[sz].LogBlockInfo();
			if (ptr)
			{
				st.push_back(ptr);
			}
		}
		else
		{
			if (st.size())
			{
				p.Free(sz, st[st.size() - 1]);
				p.poolAddresses[sz].LogBlockInfo();
				st.pop_back();
			}
		}
	}

}

void PoolAllocTestFixture::RunTests()
{
	// TestDefaultConstruction();
	TestAlloc();
}

void PoolAllocTestFixture::TestAlloc()
{
	std::cout << "PoolAllocTestFixture::TestAlloc" << std::endl;
	PoolAllocator p(100000);
	// std::cout << "sizeof(uint) " << sizeof(uint) << std::endl;
	for (int i = 4; i <= p.largestBlockSize; i <<= 1)
	{
		testPool(p, i);
	}
	std::cout << "PoolAllocTestFixture::TestAlloc -- exiting" << std::endl;
}

void PoolAllocTestFixture::TestDefaultConstruction()
{
	std::cout << "PoolAllocTestFixture::TestDefaultConstruction" << std::endl;
	PoolAllocator p(100000);
	std::cout << "PoolAllocTestFixture::TestDefaultConstruction -- exiting" << std::endl;
}