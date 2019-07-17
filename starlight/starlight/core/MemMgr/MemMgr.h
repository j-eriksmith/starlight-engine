#pragma once
#include <map>
#include "Pool.h"

/*
	Author: Alejandro Valdes
	Date: July 2019
*/

/*
	MemMgr: A Basic Memory Management System for the Starlight Game Engine.
	As of now, the MemMgr class only supports pool allocation. However, stack
	allocation will soon be supported to minimize waste when allocating 
	resources of arbitrary sizes.

	Usage Notes:
	1. The alloc() function is to be used for both normal and array allocation; simply pass the size of the resource you
	want to allocate into alloc() to allocate memory.
	2. alloc() returns void*,  meaning that you must wrap your allocations in a reinterpret_cast like below: 
	TYPE_YOU_WANT* resource = reinterpret_cast<TYPE_YOU_WANT>(memMgr.alloc(SIZE_YOU_WANT));
*/

typedef unsigned int uint;
typedef unsigned char uchar;

class MemMgr
{
public:
	explicit MemMgr(uint totalSpace);

	MemMgr(const MemMgr& rhs);

	// Allocates a resource of size 'resourceSize'.
	// [in]
	// resourceSize: Number of bytes to allocate.
	// [out]
	// Pointer to the first byte of newly allocated memory, or nullptr if resourceSize is too large.
	// NOTE: You must cast the returned address to the type that you need. See Usage Notes #2 above for more details.
	void* Alloc(uint resourceSize);

	// Frees a resource.
	// [in]
	// resourceSize: Size of allocated resource.
	// resourceAddr: Address of the first byte of the allocated region, cast to a void*.
	void Free(uint resourceSize, void* resourceAddr);

	// Returns the starting address of the pool whose block size matches the given block size.
	// [in]
	// blockSize: Desired block size.
	// [out]
	// Address of the pool matching the given block size.
	Pool* GetPoolAddress(uint blockSize);

	~MemMgr();

private:
	// Dynamically allocates a chunk of memory to use for our Pool allocator.
	// [in]
	// totalSpace: Number of bytes to allocate.
	// [out]
	// Pointer to the first byte of the newly allocated region.
	uint* AllocSpace(uint totalSpace);

	// Divides our allocated region into multiple pools.
	void BuildPools();

	// Returns the pool whose chunk size best matches the given resource's size.
	// [in]
	// resourceSize: Size of resource
	// [out]
	// Chunk size that best matches the resource size.
	uint FindBestFit(uint resourceSize) const;

	// Shifts given address to align with the given 'shift' value (must be a power of 2!).
	// [in]
	// addr: Address to shift
	// shift: Number to align with
	// [out]
	// Address that aligns with the given 'shift' value.
	static uintptr_t ShiftAddress(uintptr_t addr, uint shift);

	// shifts starting address so that the first pool will be properly aligned
	uintptr_t AlignStartAddress(uintptr_t addr);

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
