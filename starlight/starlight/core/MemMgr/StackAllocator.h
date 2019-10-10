// Expected Usage & functionality for Stack Allocator

//// interface functionality
//StackAllocator st(total_memory_in_bytes);
//
//st.alloc(space_to_alloc_in_bytes)
//
//
//// internal functionality
//
//st.clear() // resets the top of the stack to its beginning address.

//
//// Tests
//
//StackAllocator st(128);
//
////st.alloc should return a void* to a block of 128 bytes of memory
//UserType* ptr = reinterpret_cast<UserType*>(st.alloc(32 * sizeof(int)));
//
//// should return nullptr because the stack is out of memory
//UserType* ptr = reinterpret_cast<UserType*>(st.alloc(sizeof(int)));
#pragma once
#include "MemMgrTypes.h"

class StackAllocator
{

public:
	StackAllocator(uint size);
	StackAllocator(uint8_t* start, uint size);
	void* Alloc(uint size);
	void PrintStackInfo();
	~StackAllocator();

private:
	void Clear();

private:
	// holds the pointer to our dynamically allocated chunk of memory
	uint8_t* start;
	uint capacity;
	uint8_t* top;
};

