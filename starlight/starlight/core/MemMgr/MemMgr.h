#pragma once
#include <map>
#include "Pool.h"
#include "MemMgrTypes.h"
#include "PoolAllocator.h"
#include "StackAllocator.h"
#include "DoubleBufferAllocator.h"

/*
	Author: Alejandro Valdes
	Date: July - October 2019
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
	3. If you attempt to alloc() memory on a fully occupied allocator, you will receive a nullptr. It is important to check if alloc()
	has returned a valid address before attempting to dereference and write into it.
*/

class MemMgr
{
public:
	static enum class AllocatorType
	{
		FrameData,
		LevelData,
		GlobalData,
		PoolData
	};

public:
	// Allocates a resource of size 'resourceSize'.
	// [in]
	// resourceSize: Number of bytes to allocate.
	// [out]
	// Pointer to the first byte of newly allocated memory, or nullptr if resourceSize is too large.
	// NOTE: You must cast the returned address to the type that you need. See Usage Notes #2 above for more details.
	static uint8_t* Alloc(uint resourceSize, AllocatorType type);

	// Creates a MemMgr instance, default is set to 50 MB
	static void Create(uint totalSpace = 52450000);

	// Frees a resource.
	// [in]
	// resourceSize: Size of allocated resource.
	// resourceAddr: Address of the first byte of the allocated region, cast to a void*.
	static void Free(uint sz, uint8_t* res);

	~MemMgr();

private:

	explicit MemMgr(uint totalSpace);

	MemMgr(const MemMgr& rhs);

	// Dynamically allocates a chunk of memory to use for our Pool allocator.
	// [in]
	// totalSpace: Number of bytes to allocate.
	// [out]
	// Pointer to the first byte of the newly allocated region.
	uint8_t* AllocSpace(uint totalSpace);
	// Singleton MemMgr instance that performs all allocations and frees.
	static std::unique_ptr<MemMgr> memMgr;

	std::unique_ptr<DoubleBufferAllocator> frameData;
	std::unique_ptr<StackAllocator> globalData;
	std::unique_ptr<StackAllocator> levelData;
	std::unique_ptr<PoolAllocator> poolData;

	static std::unique_ptr<std::unordered_map<uint8_t*, MemMgr::AllocatorType>> addressToAllocatorMap;

	// Todo: Add double buffered stack allocator to store frame data

	// total size of the memory manager. Padding of 'largestBlockSize' added
	// to allow for proper alignment
	uint regionSize;

	// starting address
	uint8_t* start;
};


