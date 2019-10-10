#pragma once
#include "MemMgr.h"

class MemMgrTestFixture
{
public:
	void RunTests();

	void TestConstructor();

	void TestPoolData();

	void TestLevelData();

	void TestGlobalData();

	void TestFrameData();
};