#include "MemMgrTestFixture.h"

void MemMgrTestFixture::RunTests()
{
	
	TestConstructor();
}

void MemMgrTestFixture::TestConstructor()
{
	// Initialize static MemMgr object
	std::cout << "MemMgrTestFixture::TestConstructor -- Initializing static MemMgr object" << std::endl;
	MemMgr::Create();
	// Attempt to initialize it again - MemMgr should log an error and refuse this operation.
	std::cout << "MemMgrTestFixture::TestConstructor -- Attempting to initialize static MemMgr object again" << std::endl;
	MemMgr::Create();
}