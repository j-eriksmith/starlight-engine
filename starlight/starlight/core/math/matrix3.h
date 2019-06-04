#pragma once
#include "vec3.h"

/* Column-major:
*/
class Matrix3
{
public:
	Vector3 x, y, z;

	Matrix3(); // Identity matrix
	Matrix3(Vector3 v); // Diagonal matrix
	Matrix3(Vector3 inX, Vector3 inY, Vector3 inZ);

	float Determinant() const;
	Vector3 GetEulerAngle() const;
	Vector3 GetScale() const;
	Matrix3 Inverse() const;
	Matrix3 Transpose() const;
	Matrix3 Scale(const Vector3& scalar) const;

	// Axis of rotation must be normalized!
	Matrix3 Rotate(Vector3 axis, float angle) const;

	friend Matrix3 operator*(float scalar, Matrix3 m);
	friend Matrix3 operator*(Matrix3 m, float scalar);

	bool operator==(const Matrix3& rhs);
	Matrix3 operator*(const Matrix3& rhs);

	static void RunTests();
};