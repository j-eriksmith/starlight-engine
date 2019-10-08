#include "math/vec3.h"
#include <cassert>

void Vector3::RunTests()
{
	Vector3 A(1.0f, 2.0f, 3.0f), B(1.0f, 5.0f, 7.0f);

	// Cross Product 
	Vector3 Goal(-1.0f, -4.0f, 3.0f);
	assert(A.Cross(B) == Goal);

	// Dot Product 
	assert(A.Dot(B) == 32.0f);

	// Lerp
	Goal = Vector3(1.0f, 2.75f, 4.0f);
	assert(A.Lerp(B, 0.25) == Goal);

	// Slerp
	//B = A.Slerp(A, 1.0);
	//assert(A.Slerp(A, 1.0f) == A);

	// Project
	Goal = Vector3(0.42667f, 2.13333f, 2.98667f);
	assert(A.Project(B) == Goal);

	// Reflect
	Goal = Vector3(-1.0f, 2.0f, 3.0f);
	assert(A.Reflect(Vector3(1.0f, 0.0f, 0.0f)) == Goal);
}