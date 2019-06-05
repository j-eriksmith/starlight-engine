#include "Windows.h"
#include <iostream>
#include "vec3.h"
#include "matrix3.h"
#include "transform.h"

int main()
{
	Vector3::RunTests();
	Matrix3::RunTests();
	Transform::RunTests();
	return 0;
}