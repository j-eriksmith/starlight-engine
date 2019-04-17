#include "../core/math/matrix3.h"
#include <cassert>
#include <iostream>


void Matrix3::RunTests()
{
	// Inversion

	// Identity matrix inverted is itself
	Matrix3 start;
	assert(start == start.Inverse()); 

	// Identity matrix with fuzzy equality (floating points with fractions)
	start = Matrix3(Vector3(1, 4, 2), Vector3(2, 5, 7), Vector3(3, 6, 9)).Inverse();
	assert(start == Matrix3(Vector3(1 / 3, -8 / 3, 2), Vector3(1 / 3, 1 / 3, -1 / 3), Vector3(-1 / 3, 2 / 3, -1 / 3)));
}