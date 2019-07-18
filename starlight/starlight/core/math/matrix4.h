#pragma once
#include "vec4.h"

/* Column-major:
*/
class Matrix4
{
public:
	Vector4 x, y, z, w;

	Matrix4(); // Identity matrix
	Matrix4(const Vector4& v); // Diagonal matrix
	Matrix4(Vector4 inX, Vector4 inY, Vector4 inZ, Vector4 inW);

	float Determinant() const;
	Vector4 GetScale() const;
	Matrix4 Inverse() const;
	Matrix4 Transpose() const;
	Matrix4 Scale(const Vector4& scalar) const;

	// Axis of rotation must be normalized!
	Matrix4 Rotate(Vector4 axis, float angle) const;

	friend Matrix4 operator*(float scalar, Matrix4 m);
	friend Matrix4 operator*(Matrix4 m, float scalar);

	bool operator==(const Matrix4& rhs);
	Matrix4 operator*(const Matrix4& rhs);

	static void RunTests();
};
