#pragma once
#include "Debug.h"
#include "DoubleBufferAllocTestFixture.h"
#include "DoubleBufferAllocator.h"

class DoubleBufferAllocTestFixture
{
public:
	void RunTests();

private:
	void TestAllocAndSwap(uint sz = 10000);
};