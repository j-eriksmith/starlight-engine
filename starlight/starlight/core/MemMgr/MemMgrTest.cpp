#include "MemMgr.h"
// used for uintptr_t
#include <iostream>
#include <cstdint>
#include <vector>
#include <cstring>
#include "test/PoolAllocTestFixture.h"
#include "test/MemMgrTestFixture.h"
#include "test/StackAllocTestFixture.h"
#include "test/DoubleBufferAllocTestFixture.h"

/*
	Author: Alejandro Valdes
	Date: October 2019
*/


//int main()
//{
//
//	MemMgrTestFixture m;
//	m.RunTests();
//	StackAllocTestFixture st;
//	st.RunTests();
//	DoubleBufferAllocTestFixture d;
//	d.RunTests();
//	PoolAllocTestFixture p;
//	p.RunTests();
//
//	return 0;
//}