#pragma once
#include <iostream>
#include <vector>
#include <cstdint>

typedef unsigned int uint;
typedef unsigned char uchar;

class Pool
{
public:

	explicit Pool();
	Pool(uint rSize, uint bSize, uint* s);

	Pool(const Pool& rhs);

	Pool& operator = (const Pool& rhs);

	void* alloc();

	void free(void* resourceAddr);

	~Pool();

private:

	// total size of the pool
	uint regionSize;
	// size of each block in the pool (must be > 4 bytes)
	uint blockSize;
	// total number of blocks that this pool can allocate
	uint totalBlocks;
	// # of blocks available for allocation in the memory pool
	uint freeBlocks;
	// # of blocks initialized and ready for allocation
	uint initializedBlocks;
	// points to the first byte in the pool, will be converted during allocation
	uint* start;
	// head of free list. Stores the index of the next free block
	// at initialization *head = 1
	uint* head;



	// retrieves the relative index of the free list's head block
	uint getBlockIndex(uint* addr);
};