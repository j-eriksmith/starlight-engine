#pragma once
#include "PoolAllocator.h"

class PoolAllocTestFixture
{
public:
	void FillPool(Pool& p, std::vector<uint8_t*>& addrs);

	void FreePool(Pool& p, std::vector<uint8_t*>& addrs);

	void RunTests();

	void TestAllocFree();

	void TestDefaultConstruction();

	void TestFindBestFit(int reps = 100);

	void TestGetClosestPowerOfTwo(int reps = 100);

	void TestOverflowAllocFree();

private:
	static void TestPool(PoolAllocator& p, uint sz);
};