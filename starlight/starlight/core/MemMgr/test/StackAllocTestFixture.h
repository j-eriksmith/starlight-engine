#pragma once
#include "MemMgrTypes.h"

class StackAllocTestFixture
{
public:
	void RunTests();

private:
	void TestAlloc(uint sz = 1024);

	void TestConstructor(uint sz = 1024);
};