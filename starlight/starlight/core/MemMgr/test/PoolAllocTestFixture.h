#pragma once
#include "PoolAllocator.h"

class PoolAllocTestFixture
{
public:

	void RunTests();

	void TestAlloc();

	void TestDefaultConstruction();

	void TestGetClosestPowerOfTwo();

	void TestFindBestFit();

	void TestShiftAddress();

	void TestAlignStartingAddress();

private:
	static void testPool(PoolAllocator& p, uint sz);

	//static void test_128(PoolAllocator& p, uint sz);

	//static void test_32(PoolAllocator& p, uint sz);
};