#include "PoolAllocator.h"

PoolAllocator::PoolAllocator(uint totalSpace, uint blockSize)
	: start(AllocSpace(capacity)),
	largestBlockSize(blockSize),
	capacity((GetClosestPowerOfTwo(totalSpace) << 1) + largestBlockSize)
{
	// std::cout << "allocated block of size: " << capacity << "at address: " << start << std::endl;
	std::cout << "PoolAllocator::PoolAllocator -- capacity is " << capacity << " bytes." << std::endl;
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
		std::cout << "Resource too large to allocate." << std::endl;
		return nullptr;
	}
	Pool* bestFitPool = GetPoolAddress(bestFitIndex);
	return bestFitPool->Alloc();
}

void PoolAllocator::Free(uint resourceSize, void* resourceAddr)
{
	Pool* bestFitPool = GetPoolAddress(FindBestFit(resourceSize, largestBlockSize));
	bestFitPool->Free(resourceAddr);
}

Pool* PoolAllocator::GetPoolAddress(uint blockSize)
{
	// add error handling
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
	std::cout << "PoolAllocator::BuildPools" << std::endl;
	std::cout << "size: " << capacity << std::endl;
	uint8_t* current = reinterpret_cast<uint8_t*>(AlignStartAddress(reinterpret_cast<uintptr_t>(start)));
	std::cout << "current: " << reinterpret_cast<uintptr_t>(current) << std::endl;
	uint alignedPoolSize = GetClosestPowerOfTwo(reinterpret_cast<uintptr_t>(start + capacity) - reinterpret_cast<uintptr_t>(current));
	std::cout << "aligned pool size: " << alignedPoolSize << std::endl;
	uint poolSize = alignedPoolSize / 2;
	// build a pool, initialize its starting address as 'current'
	// update current to point to the next free address, repeat.

	// create multiple, starting with 1-kibibyte-sized blocks and ending with 1-byte-sized blocks
	for (uint i = largestBlockSize; i >= 4; i >>= 1)
	{
		//build pool, initialize block size to 'i' bytes
		std::cout << "current addr: " << reinterpret_cast<uintptr_t>(current) << std::endl;
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
	std::cout << "PoolAllocator::GetClosestPowerOfTwo -- original size: " << sz << std::endl;
	while ((sz & (sz - 1)))
	{
		sz &= sz - 1;
	}
	std::cout << "PoolAllocator::GetClosestPowerOfTwo -- new size: " << sz << std::endl;
	return sz;
}

uint PoolAllocator::FindBestFit(uint resourceSize, uint largestBlockSize)
{
	std::cout << "PoolAllocator::FindBestFit -- resourceSize: " << resourceSize << std::endl;
	for (uint i = 4; i <= largestBlockSize; i <<= 1)
	{
		if (i >= resourceSize)
		{
			std::cout << "PoolAllocator::FindBestFit -- best fit for resourceSize: " << i << std::endl;
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

