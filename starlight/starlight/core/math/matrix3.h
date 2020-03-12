#pragma once
#include "vec3.h"

/* Column-major:
*/
class Matrix3
{
public:
	Vector3 x, y, z;

	Matrix3(); // Identity matrix
	Matrix3(const Vector3& v); // Diagonal matrix
	Matrix3(const Vector3& inX, const Vector3& inY, const Vector3& inZ);
	Matrix3(const Vector3& dir, const Vector3& up); // from rotation

	float Determinant() const;
	Vector3 GetEulerAngle() const;
	Vector3 GetScale() const;
	Matrix3 Inverse() const;
	Matrix3 Transpose() const;
	Matrix3 Scale(const Vector3& scalar) const;

	// Axis of rotation must be normalized!
	Matrix3 Rotate(const Vector3& axis, const float& angle) const;

	friend Matrix3 operator*(const float& scalar, const Matrix3& m);
	friend Matrix3 operator*(const Matrix3& m, const float& scalar);

	bool operator==(const Matrix3& rhs);
	Matrix3 operator*(const Matrix3& rhs);

	static void RunTests();
};