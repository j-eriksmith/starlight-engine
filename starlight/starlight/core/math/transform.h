#pragma once
#include "vec3.h"
#include "matrix3.h"

class Transform
{
public:
	Vector3 Origin;
	Matrix3	Basis;

	Transform(Vector3 inX, Vector3 inY, Vector3 inZ, Vector3 inOrigin);
	Transform(Matrix3 inBasis, Vector3 inOrigin);

	Transform Rotate(Vector3 axis, float angle);
	Transform Scale(Vector3 scale);
	Transform Translate(Vector3 location);

	bool operator==(const Transform& rhs);

	static void RunTests();
};