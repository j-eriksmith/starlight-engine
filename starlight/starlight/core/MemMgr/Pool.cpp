#include "Pool.h"

/*
	Author: Alejandro Valdes
	Date: July 2019
*/

Pool::Pool()
	: regionSize(0),
	blockSize(0),
	totalBlocks(0),
	freeBlocks(0),
	initializedBlocks(0),
	start(nullptr),
	head(nullptr)
{}

Pool::Pool(uint rSize, uint bSize, uint8_t* s)
	: regionSize(rSize),
	blockSize(bSize),
	totalBlocks(rSize / bSize),
	freeBlocks(rSize / bSize),
	initializedBlocks(0),
	start(s),
	head(start) // beginning of free list will == start at pool initialization
{
	//set the index of current block
	// std::cout << "constructing pool with block size " << blockSize << " with start address at " << start << std::endl;
	*reinterpret_cast<uint*>(head) = 1;
	// std::cout << "ctor, value of head is " << *head << std::endl;
}

Pool::Pool(const Pool& rhs)
	: regionSize(rhs.regionSize),
	blockSize(rhs.blockSize),
	totalBlocks(rhs.totalBlocks),
	freeBlocks(rhs.freeBlocks),
	initializedBlocks(rhs.initializedBlocks),
	start(rhs.start),
	head(rhs.head) // beginning of free list will == start at pool initialization
{}

Pool& Pool::operator = (const Pool& rhs)
{
	regionSize = rhs.regionSize;
	blockSize = rhs.blockSize;
	totalBlocks = rhs.totalBlocks;
	freeBlocks = rhs.freeBlocks;
	initializedBlocks = rhs.initializedBlocks;
	start = rhs.start;
	head = start;
	// MUST RETURN POINTER TO THIS
	return *this;
}

void* Pool::Alloc()
{
	// std::cout << "entered Pool alloc" << std::endl;
	// std::cout << "free block count: " << freeBlocks << std::endl;
	if (!freeBlocks)
	{
		// @todo (maybe) add error checking, if !freeBlocks navigate to
		// another pool
		std::cerr << "No more room to allocate!" << std::endl;
		return nullptr;
	}

	// keep track of current head
	uint8_t* ret = head;

	/*
	 Point head to the address of the next free block.
	 ret holds the index of the next free block.
	 Multiplying a block's index # by blockSize gives you the 
	 block's physical address.
	*/

	/* 
	 We cast ret from an 8 bit int * to a regular unsigned int * to access 
	 the full index of the next free block. 
	*/
	head = start + *reinterpret_cast<uint*>(ret) * blockSize;

	/*
	 ret is now an initialized block ready for
	 allocation, and head points to the next item in the free list.
	*/

	/* 
	 If the below conditions are met, the next item in the linked list has
	 not yet been initialized, meaning that there is no index # stored
	 at its address. This statement stores the address of the 
	 next link, which is that of the block directly after this one, in the
	 current link. 
	*/

	/* 
	 If all of the blocks leading up to head are full, that means you need
	 to initialize a new block and add it to the free list.
	 if this conditional passes, all blocks to the left of head are alloc'd
	 and all blocks to the right are free. We need to add a new block to 
	 the free list
	 AKA we've initialized & alloc'd all the blocks up to this block, so we need to init
	 another block & add it to the free list 
	*/
	if (freeBlocks == totalBlocks - GetBlockIndex(ret) && GetBlockIndex(ret) == initializedBlocks)
	{
		// std::cout << "free list full. Adding a new block to the list" << std::endl;
		// since the new head is a newly initialized block, you need to set
		// its 'next' value.
		// add next available block to the free list by storing the index
		// of the block directly to the right of head.
		// next time alloc() is called, the block directly to the right
		// of current head will be set as the new head

		//prevent pool overflow, don't assign *head when *ret == totalBlocks
		if (*reinterpret_cast<uint*>(ret) < totalBlocks) 
		{ 
			*reinterpret_cast<uint*>(head) = *reinterpret_cast<uint*>(ret) + 1; 
		}
		++initializedBlocks;
	}
	--freeBlocks;
	// std::cout << "returning block of index: " << getBlockIndex(ret) << std::endl;
	void* vRet = ret;
	return vRet;
}

void Pool::Free(void* resourceAddr)
{
	// std::cout << "entered Pool free(" << resourceAddr << ")" << std::endl;
	uint8_t* offset = reinterpret_cast<uint8_t*>(resourceAddr);
	// if address is nullptr, or is out of Pool bounds or 
	// does not reference the start address of a block in the pool, abort.
	if (!resourceAddr || offset < start || offset >= start + regionSize || (offset - start) % blockSize != 0)
	{
		std::cout << "INVALID ADDRESS - CANNOT FREE" << std::endl;
		return;
	}
	// keep track of old free-list head
	uint8_t* oldHead = head;
	// std::cout << "old head: " << getBlockIndex(oldHead) << std::endl;
	// reassign head to newly freed block
	head = reinterpret_cast<uint8_t*>(resourceAddr);
	// std::cout << "new head: " << getBlockIndex(head) << std::endl;
	// assign contents of new head to the index of the old head
	*reinterpret_cast<uint*>(head) = GetBlockIndex(oldHead);
	// std::cout << "new head points to: " << *head << std::endl;
	++freeBlocks;
}

uint Pool::GetBlockIndex(uint8_t* addr)
{
	return (addr - start) / blockSize;
}

void Pool::LogBlockInfo()
{
	std::cout << "*** Pool::LogBlockInfo ***" << std::endl;
	std::cout << "*** freeBlock Count: " << freeBlocks << std::endl;
	std::cout << "*** initializedBlock Count: " << initializedBlocks << std::endl;
}

void Pool::LogPoolInfo()
{
	std::cout << "*** Pool::LogPoolInfo ***" << std::endl;
	std::cout << "*** regionSize: " << regionSize << std::endl;
	std::cout << "*** blockSize: " << blockSize << std::endl;
	std::cout << "*** totalBlocks: " << totalBlocks << std::endl;
	// std::cout << "*** start address: " << start << std::endl;
	// std::cout << "*** head address: " << head << std::endl;
	std::cout << "*** head index: " << GetBlockIndex(head) << std::endl;
}

Pool::~Pool()
{
	// std::cout << "Destructor Called" << std::endl;
}