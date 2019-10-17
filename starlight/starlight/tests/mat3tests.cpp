#include "math/matrix3.h"
#include <cassert>
#include <iostream>


void Matrix3::RunTests()
{
	// Inversion (uses determinant)

	// Identity matrix inverted is itself
	Matrix3 start;
	assert(start == start.Inverse()); 

	// Identity matrix with fuzzy equality (floating points with fractions)
	start = Matrix3(Vector3(1, 4, 2), Vector3(2, 5, 7), Vector3(3, 6, 9)).Inverse();
	assert(start == Matrix3(Vector3(1.f / 3.f, -8.f / 3.f, 2.f), Vector3(1.f / 3.f, 1.f / 3.f, -1.f / 3.f), Vector3(-1.f / 3.f, 2.f / 3.f, -1.f / 3.f)));
}