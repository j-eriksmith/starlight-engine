#include "MemMgrTestFixture.h"

void MemMgrTestFixture::RunTests()
{
	TestConstructor();
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