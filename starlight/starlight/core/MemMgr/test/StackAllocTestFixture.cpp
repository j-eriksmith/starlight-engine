#include "StackAllocTestFixture.h"
#include "StackAllocator.h"
#include <chrono>
#include <random>
#include <vector>
#include <cassert>

void StackAllocTestFixture::RunTests()
{
	TestConstructor();
	TestAlloc();
}
void StackAllocTestFixture::TestAlloc(uint sz)
{
	StackAllocator* st = new StackAllocator(sz);
	uint seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 random(seed);
	std::uniform_int_distribution<int> distribution(1, sz / 4);
	// 50 is an arbitrary number of reps to run randomized allocation on our StackAllocator
	for (int i = 0; i < 50; ++i)
	{
		uint8_t* curPtr = reinterpret_cast<uint8_t*>(st->Alloc(distribution(random)));
		while (curPtr)
		{
			curPtr = reinterpret_cast<uint8_t*>(st->Alloc(distribution(random)));
			assert(curPtr < st->start + st->capacity);
		}
		st->Clear();
		curPtr = reinterpret_cast<uint8_t*>(st->Alloc(distribution(random)));
		assert(curPtr == st->start);
	}

}

void StackAllocTestFixture::TestConstructor(uint sz)
{
	// Test dynamic allocation of Stack Allocator
	StackAllocator* st1 = new StackAllocator(sz);
	// Test stack allocation of Stack Allocator
	StackAllocator st(sz);
	delete st1;
}