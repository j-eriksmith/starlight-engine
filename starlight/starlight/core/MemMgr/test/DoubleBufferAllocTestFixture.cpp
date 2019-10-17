#include "DoubleBufferAllocTestFixture.h"
#include <unordered_map>
#include <chrono>
#include <random>
#include <cassert>

void DoubleBufferAllocTestFixture::RunTests()
{
	TestAllocAndSwap();
}

void DoubleBufferAllocTestFixture::TestAllocAndSwap(uint sz)
{
	DoubleBufferAllocator* d = new DoubleBufferAllocator(sz);
	uint seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 random(seed);
	std::uniform_int_distribution<int> distribution(2, std::numeric_limits<int>::max());
	
	std::unordered_map<uint8_t*, int> vals;
	uint8_t* curPtr = reinterpret_cast<uint8_t*>(d->Alloc(4));
	int curVal;
	while (curPtr)
	{
		curVal = distribution(random);
		*reinterpret_cast<int*>(curPtr) = curVal;
		vals[curPtr] = curVal;
		curPtr = reinterpret_cast<uint8_t*>(d->Alloc(4));
	}
	// First buffer should be full, so no more allocations should be allowed.
	assert(d->Alloc(1) == nullptr);
	d->Swap();
	curPtr = reinterpret_cast<uint8_t*>(d->Alloc(4));
	// Fill up the other buffer.
	while (curPtr)
	{
		// Set all values in our second buffer to 1. Since our first buffer
		// will not contain any blocks with the value 1, setting all blocks to 1 
		// will help us validate that no values in the first buffer were overwritten.
		*reinterpret_cast<int*>(curPtr) = 1;
		curPtr = reinterpret_cast<uint8_t*>(d->Alloc(4));
	}
	// Check the first buffer, ensure that the values we assigned into our allocated blocks
	// have not been overwritten.
	for (std::pair<uint8_t*, int> val : vals)
	{
		assert(*reinterpret_cast<int*>(val.first) == val.second);
	}
}