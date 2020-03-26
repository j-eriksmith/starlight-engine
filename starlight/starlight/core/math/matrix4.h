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
	Matrix4(const Vector4& inX, const Vector4& inY, const Vector4& inZ, const Vector4& inW);
	Matrix4(const Matrix4& rhs);
	float Determinant() const;
	Vector4 GetScale() const;
	Matrix4 Inverse() const;
	Matrix4 Transpose() const;
	Matrix4 Scale(const Vector4& scalar) const;

	friend Matrix4 operator*(const float& scalar, const Matrix4& m);
	friend Matrix4 operator*(const Matrix4& m, const float& scalar);

	bool operator==(const Matrix4& rhs);
	Matrix4 operator*(const Matrix4& rhs);

	static void RunTests();
};
