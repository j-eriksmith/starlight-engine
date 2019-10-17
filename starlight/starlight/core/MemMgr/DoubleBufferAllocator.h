#pragma once
#include "StackAllocator.h"
#include "MemMgrTypes.h"

class DoubleBufferAllocator
{
public:
	DoubleBufferAllocator(int sz);

	~DoubleBufferAllocator();

	void* Alloc(uint sz);

	void Swap();

private:
	int currentBuffer;
	StackAllocator** buf;
	
	friend class DoubleBufferAllocTestFixture;
};