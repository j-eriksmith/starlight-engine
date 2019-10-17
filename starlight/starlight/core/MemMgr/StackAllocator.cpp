#include "StackAllocator.h"
#include <iostream>
#include "Debug.h"
StackAllocator::StackAllocator(uint sz)
	: start(new uint8_t[sz]),
	capacity(sz * sizeof(uint8_t)),
	top(start)
    
{
	Log("StackAllocator::StackAllocator -- Created new chunk with size " << sz);
	PrintStackInfo();
}

StackAllocator::~StackAllocator()
{
	delete[] start;
}

StackAllocator::StackAllocator(uint8_t* st, uint sz)
	:start(st),
	capacity(sz),
	top(st)
{
	Log("StackAllocator::StackAllocator -- Was given chunk starting at " << st << "with size " << sz);
	PrintStackInfo();
}

void* StackAllocator::Alloc(uint size)
{
	if (!size)
	{
		Log("StackAllocator::Alloc -- Invalid allocation size of " << size);
		return nullptr;
	}
	else if (top + size > start + capacity)
	{
		Log("StackAllocator::Alloc -- Not enough room to allocate block of size " << size);
		return nullptr;
	}

	uint8_t* temp = top;
	top += size;
	return temp;
}

void StackAllocator::Clear()
{
	top = start;
}

void StackAllocator::PrintStackInfo()
{
	Log("Total Stack capacity: " << capacity);
	Log("buf pointer location: " << reinterpret_cast<uintptr_t>(start));
	Log("top pointer location: " << reinterpret_cast<uintptr_t>(start));
}