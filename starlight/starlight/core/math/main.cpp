#include "Windows.h"
#include <iostream>
#include "vec3.h"
#include "vec4.h"
#include "matrix3.h"
#include "matrix4.h"
#include "transform.h"

int main()
{
	Vector3::RunTests();
	Matrix3::RunTests();
	Matrix4::RunTests();
	Transform::RunTests();
	return 0;
}