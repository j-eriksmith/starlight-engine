#include "StackAllocator.h"
#include <iostream>

StackAllocator::StackAllocator(uint sz)
	: start(new uint8_t[sz]),
	capacity(sz * sizeof(uint8_t)),
	top(start)
    
{
	std::cout << "StackAllocator::StackAllocator -- Created new chunk with size " << sz << std::endl;
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
	std::cout << "StackAllocator::StackAllocator -- Was given chunk starting at " << st << "with size " << sz << std::endl;
	PrintStackInfo();
}

void* StackAllocator::Alloc(uint size)
{
	if (top + size > start + capacity)
	{
		std::cerr << "StackAllocator::Alloc -- Not enough room to allocate block of size " << size << std::endl;
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
	std::cout << "Total Stack capacity: " << capacity << std::endl;
	std::cout << "buf pointer location: " << reinterpret_cast<uintptr_t>(start) << std::endl;
	std::cout << "top pointer location: " << reinterpret_cast<uintptr_t>(start) << std::endl;
}