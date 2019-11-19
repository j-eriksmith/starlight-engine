#include "StackAllocator.h"
#include <iostream>

//int main()
//{
//	StackAllocator st(128);
//	int* A = reinterpret_cast<int*>(st.Alloc(10 * sizeof(int)));
//	for (int i = 0; i < 10; ++i)
//	{
//		A[i] = i;
//	}
//	for (int i = 0; i < 10; ++i)
//	{
//		std::cout << A[i] << " ";
//	}
//
//	std::cout << std::endl;
//	return 0;
//}