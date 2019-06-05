#pragma once
#include <map>
#include "Pool.h"

typedef unsigned int uint;
typedef unsigned char uchar;

class MemMgr
{
public:
	explicit MemMgr(uint totalSpace);

	MemMgr(const MemMgr& rhs);

	// 
	void* alloc(uint resourceSize);

	void free(uint resourceSize, void* resourceAddr);

	uint* allocSpace(uint totalSpace);

	void BuildPools();

	uint findBestFit(uint resourceSize);

	Pool* getPoolAddress(uint blockSize);

	static uintptr_t shiftAddress(uintptr_t addr, uint shift);

	// shifts starting address so that the first pool will be properly aligned
	uintptr_t alignStartAddress(uintptr_t addr);


	~MemMgr();

private:
	// total size of the memory manager. Padding of 'largestBlockSize' added
	// to allow for proper alignment
	uint regionSize;

	// starting address
	uint* start;

	// largest block size that our allocator allows, in bytes
	// MUST BE A POWER OF 2 SO THAT THE POOLS CAN BE ALIGNED PROPERLY
	static const uint largestBlockSize = 1024;

	// maps a pool's block size to its address
	std::map<uint, Pool> poolAddresses;
};
