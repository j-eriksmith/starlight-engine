#include "MemMgr.h"

MemMgr::MemMgr(uint totalSpace)
	: regionSize(totalSpace + largestBlockSize),
	start(allocSpace(regionSize))
{
	std::cout << "allocated block of size: " << regionSize << "at address: " << start << std::endl;
	BuildPools();
}

MemMgr::MemMgr(const MemMgr& rhs)
	: regionSize(rhs.regionSize),
	start(rhs.start),
	poolAddresses(rhs.poolAddresses)
{}

void* MemMgr::alloc(uint resourceSize)
{
	// @todo: investigate how converting a user defined object pointer into a
	// uint * will affect the integrity of the ptr
	//uintptr_t correctPool = findBestFitPool(itemSize);
	// return address of an open block in the requested pool
	//return reinterpret_cast<Pool *>(correctPool)->alloc();
	std::cout << "called 'MemMgr.alloc(), attempting to alloc size of " << resourceSize << std::endl;
	uint bestFitIndex = findBestFit(resourceSize);
	std::cout << "found best fit size of " << bestFitIndex << std::endl;
	if (!bestFitIndex)
	{
		std::cout << "Resource too large to allocate." << std::endl;
		return nullptr;
	}
	Pool* bestFitPool = getPoolAddress(bestFitIndex);
	return bestFitPool->alloc();
}

void MemMgr::free(uint resourceSize, void* resourceAddr)
{
	Pool* bestFitPool = getPoolAddress(findBestFit(resourceSize));
	bestFitPool->free(resourceAddr);
}

uint* MemMgr::allocSpace(uint totalSpace)
{
	// returns array of 'totalSpace' bytes, ready to split into pools
	return new uint[totalSpace];
}

void MemMgr::BuildPools()
{
	// starting address of each pool (updated as you build each pool)
	// align current address, prepare to assign starting addresses for pools
	// convert start into uintptr_t to perform bit shifting operations, then convert it back to uint*
	uint* current = reinterpret_cast<uint*>(alignStartAddress(reinterpret_cast<uintptr_t>(start)));
	std::cout << "Starting address shifted to: " << current << std::endl;
	uint poolSize = regionSize / 2;
	// build a pool, initialize its starting address as 'current'
	// update current to point to the next free address, repeat.

	// create multiple, starting with 1-kibibyte-sized blocks and ending with 1-byte-sized blocks
	for (uint i = largestBlockSize; i >= 4; i /= 2)
	{
		//build pool, initialize block size to 'i' bytes
		Pool currentPool = Pool(poolSize, i, current);
		//*reinterpret_cast<Pool*>(current) = Pool(poolSize, i, current);
		std::cout << " Size of pool is: " << poolSize
			<< " for blocks of size: " << i
			<< " bytes allocated at address: "
			<< current << ". ";
		// add a reference to this pool to the poolAddresses map
		poolAddresses.insert(std::pair<uint, Pool>(i, currentPool));
		// next pool's address will be directly behind this iteration's pool
		current += poolSize;
		// the block and pool size decrease at the same rate (log2) so that 
		// all allocated pools are given a proportional number of available
		// block spaces
		poolSize /= 2;
	}
}

uint MemMgr::findBestFit(uint resourceSize)
{
	for (uint i = 4; i <= largestBlockSize; i *= 2)
	{
		if (i >= resourceSize)
		{
			return i;
		}
	}
	// could not find a pool large enough to allocate this resource
	return 0;
}

Pool* MemMgr::getPoolAddress(uint blockSize)
{
	// add error handling
	return &poolAddresses[blockSize];
}

uintptr_t MemMgr::shiftAddress(uintptr_t addr, uint shift)
{
	int mask = shift - 1;
	uintptr_t newAddr = addr + mask;
	return newAddr & ~mask;
}

// shifts starting address so that the first pool will be properly aligned
uintptr_t MemMgr::alignStartAddress(uintptr_t addr)
{
	return shiftAddress(addr, largestBlockSize);
}

MemMgr::~MemMgr()
{
	delete[] start;
}




