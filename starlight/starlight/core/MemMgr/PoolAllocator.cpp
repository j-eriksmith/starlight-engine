#include "PoolAllocator.h"

PoolAllocator::PoolAllocator(uint totalSpace, uint blockSize)
	: start(AllocSpace(capacity)),
	largestBlockSize(blockSize),
	capacity((GetClosestPowerOfTwo(totalSpace) << 1) + largestBlockSize)
{
	// Log("allocated block of size: " << capacity << "at address: " << start);
	Log("PoolAllocator::PoolAllocator -- capacity is " << capacity << " bytes.");
	BuildPools();
}

PoolAllocator::PoolAllocator(const PoolAllocator& rhs)
	: capacity(rhs.capacity),
	start(rhs.start),
	poolAddresses(rhs.poolAddresses),
	largestBlockSize(rhs.largestBlockSize)
{}

PoolAllocator::~PoolAllocator()
{
	delete[] start;
}

void* PoolAllocator::Alloc(uint resourceSize)
{
	uint bestFitIndex = FindBestFit(resourceSize, largestBlockSize);
	if (!bestFitIndex)
	{
		Log("Resource too large to allocate.");
		return nullptr;
	}
	Pool* bestFitPool = GetPoolAddress(bestFitIndex);
	uint8_t* ret = reinterpret_cast<uint8_t*>(bestFitPool->Alloc());
	while (!ret)
	{
		bestFitIndex <<= 1;
		if (bestFitIndex > largestBlockSize)
		{
			Log("PoolAllocator::Alloc -- No space left in allocator to store this block.");
			return nullptr;
		}
		Log("PoolAllocator::Alloc -- Allocated resource of size " << resourceSize << " into block of size " << bestFitIndex);
		ret = reinterpret_cast<uint8_t*>(GetPoolAddress(bestFitIndex)->Alloc());
	}
	return reinterpret_cast<void*>(ret);
}

void PoolAllocator::Free(uint resourceSize, void* resourceAddr)
{
	uint bestFit = FindBestFit(resourceSize, largestBlockSize);
	for (int i = bestFit; i <= largestBlockSize; i <<= 1)
	{
		if (poolAddresses[i].IsValidAddress(resourceAddr))
		{
			Log("PoolAllocator::Free -- Freeing block from pool with blockSize " << i);
			poolAddresses[i].Free(resourceAddr);
			return;
		}
	}
	Log("PoolAllocator::Free -- Could not free block of size " << resourceSize 
		<< " at address " << reinterpret_cast<uintptr_t>(resourceAddr));
}

Pool* PoolAllocator::GetPoolAddress(uint blockSize)
{
	if (poolAddresses.find(blockSize) == poolAddresses.end())
	{
		Log("PoolAllocator::GetPoolAddress -- pool of size " << blockSize << " not found.");
		return nullptr;
	}
	return &(poolAddresses[blockSize]);
}

uint8_t* PoolAllocator::AllocSpace(uint totalSpace)
{
	// returns array of 'totalSpace' bytes, ready to split into pools
	// allocate an extra 'largestBlockSize' bytes to accomodate for bit alignment that we perform in BuildPools
	return new uint8_t[totalSpace + largestBlockSize];
}

void PoolAllocator::BuildPools()
{
	// starting address of each pool (updated as you build each pool)
	// align current address, prepare to assign starting addresses for pools
	// convert start into uintptr_t to perform bit shifting operations, then convert it back to uint*
	Log("PoolAllocator::BuildPools");
	Log("size: " << capacity);
	uint8_t* current = reinterpret_cast<uint8_t*>(AlignStartAddress(reinterpret_cast<uintptr_t>(start)));
	Log("current: " << reinterpret_cast<uintptr_t>(current));
	uint alignedPoolSize = GetClosestPowerOfTwo(reinterpret_cast<uintptr_t>(start + capacity) - reinterpret_cast<uintptr_t>(current));
	Log("aligned pool size: " << alignedPoolSize);
	uint poolSize = alignedPoolSize / 2;
	// build a pool, initialize its starting address as 'current'
	// update current to point to the next free address, repeat.

	// create multiple, starting with 1-kibibyte-sized blocks and ending with 1-byte-sized blocks
	for (uint i = largestBlockSize; i >= 4; i >>= 1)
	{
		//build pool, initialize block size to 'i' bytes
		Log("current addr: " << reinterpret_cast<uintptr_t>(current));
		Pool currentPool = Pool(poolSize, i, current);
		// add a reference to this pool to the poolAddresses map
		poolAddresses.insert(std::pair<uint, Pool>(i, currentPool));
		// next pool's address will be directly behind this iteration's pool
		current += poolSize;
		// the block and pool size decrease at the same rate (log2) so that 
		// all allocated pools are given a proportional number of available
		// block spaces
		poolSize >>= 1;
	}
}

uint PoolAllocator::GetClosestPowerOfTwo(uint sz)
{
	Log("PoolAllocator::GetClosestPowerOfTwo -- original size: " << sz);
	while ((sz & (sz - 1)))
	{
		sz &= sz - 1;
	}
	Log("PoolAllocator::GetClosestPowerOfTwo -- new size: " << sz);
	return sz;
}

uint PoolAllocator::FindBestFit(uint resourceSize, uint largestBlockSize)
{
	Log("PoolAllocator::FindBestFit -- resourceSize: " << resourceSize);
	for (uint i = 4; i <= largestBlockSize; i <<= 1)
	{
		if (i >= resourceSize)
		{
			Log("PoolAllocator::FindBestFit -- best fit for resourceSize: " << i);
			return i;
		}
	}
	// could not find a pool large enough to allocate this resource
	return 0;
}

uintptr_t PoolAllocator::ShiftAddress(uintptr_t addr, uint shift)
{
	int mask = shift - 1;
	uintptr_t newAddr = addr + mask;
	return newAddr & ~mask;
}

// shifts starting address so that the first pool will be properly aligned
uintptr_t PoolAllocator::AlignStartAddress(uintptr_t addr)
{
	return ShiftAddress(addr, largestBlockSize);
}

