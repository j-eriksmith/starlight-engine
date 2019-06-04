#include "Pool.h"



Pool::Pool()
	: regionSize(0),
	blockSize(0),
	totalBlocks(0),
	freeBlocks(0),
	initializedBlocks(0),
	start(nullptr),
	head(nullptr)
{}

Pool::Pool(uint rSize, uint bSize, uint* s)
	: regionSize(rSize),
	blockSize(bSize),
	totalBlocks(rSize / bSize),
	freeBlocks(rSize / bSize),
	initializedBlocks(0),
	start(s),
	head(start) // beginning of free list will == start at pool initialization
{
	//set the index of current block
	*head = 1;
	std::cout << "ctor, value of head is " << *head << std::endl;
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

void* Pool::alloc()
{
	std::cout << "entered Pool alloc" << std::endl;
	std::cout << "free block count: " << freeBlocks << std::endl;
	if (!freeBlocks)
	{
		// @todo (maybe) add error checking, if !freeBlocks navigate to
		// another pool
		std::cerr << "No more room to allocate!" << std::endl;
		return nullptr;
	}

	// keep track of current head
	uint* ret = head;
	std::cout << "old head pointed to block index: " << getBlockIndex(ret) << std::endl;
	// point head to the address of the next free block
	// ret holds the index of the next free block
	// multiplying a block's index # by blockSize gives you the 
	// block's physical address
	//std::cout << "head's value: " << *head << std::endl;
	//std::cout << "ret's value: " << *ret << std::endl;
	head = start + *ret * blockSize;
	//std::cout << "new head: " << head << " versus start: " << start << std::endl;
	std::cout << "head now points to block index: " << getBlockIndex(head) << std::endl;
	// ret is now an initialized block ready for
	// allocation, and head points to the next item in the free list


	// if the below conditions are met, the next item in the linked list has
	// not yet been initialized, meaning that there is no index # stored
	// at its address. This statement stores the address of the 
	// next link, which is that of the block directly after this one, in the
	// current link.

	// if all of the blocks leading up to head are full, that means you need
	// to initialize a new block and add it to the free list.
	// if this conditional passes, all blocks to the left of head are alloc'd
	// and all blocks to the right are free. We need to add a new block to 
	// the free list
	// AKA we've initialized & alloc'd all the blocks up to this block, so we need to init
	// another block & add it to the free list
	if (freeBlocks == totalBlocks - getBlockIndex(ret) && getBlockIndex(ret) == initializedBlocks)
	{
		std::cout << "free list full. Adding a new block to the list" << std::endl;
		// since the new head is a newly initialized block, you need to set
		// its 'next' value.
		// add next available block to the free list by storing the index
		// of the block directly to the right of head.
		// next time alloc() is called, the block directly to the right
		// of current head will be set as the new head

		//prevent pool overflow, don't assign *head when *ret == totalBlocks
		if (*ret < totalBlocks) 
		{ 
			*head = *ret + 1; 
		}
		

		++initializedBlocks;
	}
	--freeBlocks;
	std::cout << "returning block of index: " << getBlockIndex(ret) << std::endl;
	void* vRet = ret;
	return vRet;
}

void Pool::free(void* resourceAddr)
{
	std::cout << "entered Pool free(" << resourceAddr << ")" << std::endl;
	uint* offset = reinterpret_cast<uint*>(resourceAddr);
	// if address is nullptr, or is out of Pool bounds or 
	// does not reference the start address of a block in the pool, abort.
	if (!resourceAddr || offset < start || offset >= start + regionSize || (offset - start) % blockSize != 0)
	{
		std::cout << "INVALID ADDRESS - CANNOT FREE" << std::endl;
		return;
	}
	// keep track of old free-list head
	uint* oldHead = head;
	std::cout << "old head: " << getBlockIndex(oldHead) << std::endl;
	// reassign head to newly freed block
	head = reinterpret_cast<uint*>(resourceAddr);
	std::cout << "new head: " << getBlockIndex(head) << std::endl;
	// assign contents of new head to the index of the old head
	*head = getBlockIndex(oldHead);
	std::cout << "new head points to: " << *head << std::endl;
	++freeBlocks;
}

uint Pool::getBlockIndex(uint* addr)
{
	return (addr - start) / blockSize;
}

Pool::~Pool()
{
	std::cout << "Destructor Called" << std::endl;
}