#include "MemMgrTestFixture.h"
#include <cassert>
void MemMgrTestFixture::RunTests()
{
	TestConstructor();
	TestAlloc();
}

void MemMgrTestFixture::TestAlloc()
{
	MemMgr::Create();
	int* arr = reinterpret_cast<int*>(MemMgr::Alloc(sizeof(int) * 100, MemMgr::AllocatorType::LevelData));
	for (int i = 0; i < 100; ++i)
	{
		arr[i] = i;
	}
	for (int i = 99; i >= 0; --i)
	{
		assert(arr[i] == i);
	}
	MemMgr::Free(sizeof(int) * 100, reinterpret_cast<uint8_t*>(arr));
}

void MemMgrTestFixture::TestConstructor()
{
	// Initialize static MemMgr object
	Log("MemMgrTestFixture::TestConstructor -- Initializing static MemMgr object");
	MemMgr::Create();
	// Attempt to initialize it again - MemMgr should log an error and refuse this operation.
	Log("MemMgrTestFixture::TestConstructor -- Attempting to initialize static MemMgr object again");
	MemMgr::Create();
}