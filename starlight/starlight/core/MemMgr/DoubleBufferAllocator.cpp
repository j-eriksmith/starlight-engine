#include "DoubleBufferAllocator.h"

DoubleBufferAllocator::DoubleBufferAllocator(int sz)
	:currentBuffer(0), buf(new StackAllocator* [2])
{
	buf[0] = new StackAllocator(sz);
	buf[1] = new StackAllocator(sz);
}

DoubleBufferAllocator::~DoubleBufferAllocator()
{
	delete buf[0];
	delete buf[1];
	delete[] buf;
}

void* DoubleBufferAllocator::Alloc(uint sz)
{
	return buf[currentBuffer]->Alloc(sz);
}

void DoubleBufferAllocator::Swap()
{
	currentBuffer = !currentBuffer;
	buf[currentBuffer]->Clear();
}