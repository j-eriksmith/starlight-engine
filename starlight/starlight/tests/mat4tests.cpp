#include "../core/math/matrix4.h"
#include <cassert>
#include <iostream>


void Matrix4::RunTests()
{
	// Inversion (uses determinant)

	// Identity matrix inverted is itself
	Matrix4 start, other;
	assert(start == start.Inverse()); 

	// Identity matrix with fuzzy equality (floating points with .fractions)
	start = Matrix4(Vector4(1, 4, 2, 5), Vector4(2, 5, 7, 6), Vector4(3, 6, 9, 7), Vector4(8, 2, 5, 8)).Inverse();
	other = Matrix4(Vector4(19.f / 72.f, -35.f / 18.f, 103.f / 72.f, 1.f / 24.f), Vector4(11.f / 18.f, -23.f / 9.f, 35.f / 18.f, -1.f / 6.f), Vector4(-1.f / 3.f, 2.f / 3.f, -1.f / 3.f, 0.f), Vector4(-5.f / 24.f, 13.f / 6.f, -41.f / 24.f, 1.f / 8.f));

	assert(start == Matrix4(Vector4(19.f / 72.f, -35.f / 18.f, 103.f / 72.f, 1.f / 24.f), Vector4(11.f / 18.f, -23.f / 9.f, 35.f / 18.f, -1.f / 6.f), Vector4(-1.f / 3.f, 2.f / 3.f, -1.f / 3.f, 0.f), Vector4(-5.f / 24.f, 13.f / 6.f, -41.f / 24.f, 1.f / 8.f)));
}