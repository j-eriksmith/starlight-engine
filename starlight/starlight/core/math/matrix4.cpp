#include "matrix4.h"
#include "utils.h"
#include <iostream>

Matrix4::Matrix4()
	: x(Vector4(1.0f, 0.0f, 0.0f, 0.0f)),
		y(Vector4(0.0f, 1.0f, 0.0f, 0.0f)),
		z(Vector4(0.0f, 0.0f, 1.0f, 0.0f)),
		w(Vector4(0.0f, 0.0f, 0.0f, 1.0f))
{}

Matrix4::Matrix4(const Vector4& v)
	: x(v.x, 0.0f, 0.0f, 0.0f),
	y(0.0f, v.y, 0.0f, 0.0f),
	z(0.0f, 0.0f, v.z, 0.0f),
	w(0.0f, 0.0f, 0.0f, v.w)
{}

Matrix4::Matrix4(Vector4 inX, Vector4 inY, Vector4 inZ, Vector4 inW)
	: x(inX),
		y(inY),
		z(inZ),
		w(inW)
{}

float Matrix4::Determinant() const
{
	float m11 = y.y * z.z * w.w - y.y * z.w * w.z -
		z.y * y.z * w.w + z.y * y.w * w.z +
		w.y * y.z * z.w - w.y * y.w * z.z;
	float m21 = -y.x * z.z * w.w + y.x * z.w * w.z +
		z.x * y.z * w.w - z.x * y.w * w.z -
		w.x * y.z * z.w + w.x * y.w * z.z;
	float m31 = y.x * z.y * w.w - y.x * z.w * w.y -
		z.x * y.y * w.w + z.x * y.w * w.y +
		w.x * y.y * z.w - w.x * y.w * z.y;
	float m41 = -y.x * z.y * w.z + y.x * z.z * w.y +
		z.x * y.y * w.z - z.x * y.z * w.y -
		w.x * y.y * z.z + w.x * y.z * z.y;
	return x.x * m11 + x.y * m21 + x.z * m31 + x.w * m41;
}

Vector4 Matrix4::GetScale() const
{
	return Vector4(x.x, y.y, z.z, w.w);
}

Matrix4 Matrix4::Inverse() const
{
	float m11 = y.y * z.z * w.w - y.y * z.w * w.z -
		z.y * y.z * w.w + z.y * y.w * w.z +
		w.y * y.z * z.w - w.y * y.w * z.z;
	float m21 = -y.x * z.z * w.w + y.x * z.w * w.z +
		z.x * y.z * w.w - z.x * y.w * w.z -
		w.x * y.z * z.w + w.x * y.w * z.z;
	float m31 = y.x * z.y * w.w - y.x * z.w * w.y -
		z.x * y.y * w.w + z.x * y.w * w.y +
		w.x * y.y * z.w - w.x * y.w * z.y;
	float m41 = -y.x * z.y * w.z + y.x * z.z * w.y +
		z.x * y.y * w.z - z.x * y.z * w.y -
		w.x * y.y * z.z + w.x * y.z * z.y;
	float m12 = -x.y * z.z * w.w + x.y * z.w * w.z +
		z.y * x.z * w.w - z.y * x.w * w.z -
		w.y * x.z * z.w + w.y * x.w * z.z;
	float m22 = x.x * z.z * w.w - x.x * z.w * w.z -
		z.x * x.z * w.w + z.x * x.w * w.z +
		w.x * x.z * z.w - w.x * x.w * z.z;
	float m32 = -x.x * z.y * w.w + x.x * z.w * w.y +
		z.x * x.y * w.w - z.x * x.w * w.y -
		w.x * x.y * z.w + w.x * x.w * z.y;
	float m42 = x.x * z.y * w.z - x.x * z.z * w.y -
		z.x * x.y * w.z + z.x * x.z * w.y +
		w.x * x.y * z.z - w.x * x.z * z.y;
	float m13 = x.y * y.z * w.w - x.y * y.w * w.z -
		y.y * x.z * w.w + y.y * x.w * w.z +
		w.y * x.z * y.w - w.y * x.w * y.z;
	float m23 = -x.x * y.z * w.w + x.x * y.w * w.z +
		y.x * x.z * w.w - y.x * x.w * w.z -
		w.x * x.z * y.w + w.x * x.w * y.z;
	float m33 = x.x * y.y * w.w - x.x * y.w * w.y -
		y.x * x.y * w.w + y.x * x.w * w.y +
		w.x * x.y * y.w - w.x * x.w * y.y;
	float m43 = -x.x * y.y * w.z + x.x * y.z * w.y +
		y.x * x.y * w.z - y.x * x.z * w.y -
		w.x * x.y * y.z + w.x * x.z * y.y;
	float m14 = -x.y * y.z * z.w + x.y * y.w * z.z +
		y.y * x.z * z.w - y.y * x.w * z.z -
		z.y * x.z * y.w + z.y * x.w * y.z;
	float m24 = x.x * y.z * z.w - x.x * y.w * z.z -
		y.x * x.z * z.w + y.x * x.w * z.z +
		z.x * x.z * y.w - z.x * x.w * y.z;
	float m34 = -x.x * y.y * z.w + x.x * y.w * z.y +
		y.x * x.y * z.w - y.x * x.w * z.y -
		z.x * x.y * y.w + z.x * x.w * y.y;
	float m44 = x.x * y.y * z.z - x.x * y.z * z.y -
		y.x * x.y * z.z + y.x * x.z * z.y +
		z.x * x.y * y.z - z.x * x.z * y.y;

	float det = x.x * m11 + x.y * m21 + x.z * m31 + x.w * m41;

	if (det == 0) 
	{
		throw std::out_of_range("Matrix4::Inverse => Cannot do inverse when the determinant is zero.");
	}
}

Matrix4 Matrix4::Transpose() const
{
	return Matrix4(Vector4(x[0], y[0], z[0], w[0]), Vector4(x[1], y[1], z[1], w[1]), Vector4(x[2], y[2], z[2], w[2]), Vector4(x[3], y[3], z[3], w[3]));
}

Matrix4 Matrix4::Scale(const Vector4& scalar) const
{
	// Cheating with no multiplication since we know we're doing a scale operation
	return Matrix4(Vector4(x.x * scalar.x, x.y, x.z, x.w),
					Vector4(y.x, y.y * scalar.y, y.z, y.w),
					Vector4(z.x, z.y, z.z * scalar.z, z.w),
					Vector4(w.x, w.y, w.z, w.w * scalar.w));
}

Matrix4 operator*(float scalar, Matrix4 m)
{
	return Matrix4(m.x * scalar, m.y * scalar, m.z * scalar, m.w * scalar);
}

Matrix4 operator*(Matrix4 m, float scalar)
{
	return Matrix4(m.x * scalar, m.y * scalar, m.z * scalar, m.w * scalar);
}

bool Matrix4::operator==(const Matrix4& rhs)
{
	float Epsilon = 0.00001f;
	float Diff = x.x - rhs.x.x + x.y - rhs.x.y + x.z - rhs.x.z + x.w - rhs.x.w
		+ y.x - rhs.y.x + y.y - rhs.y.y + y.z - rhs.y.z + y.w - rhs.y.w 
		+ z.x - rhs.z.x + z.y - rhs.z.y + z.z - rhs.z.z + z.w - rhs.z.w
		+ w.x - rhs.w.x + w.y - rhs.z.w + w.z - rhs.w.z + w.w - rhs.w.w;
	return Utils::Abs(Diff) <= Epsilon;
}

Matrix4 Matrix4::operator*(const Matrix4& rhs)
{
	Matrix4 Trans(rhs.Transpose());

	return Matrix4(Vector4(x.Dot(Trans.x), y.Dot(Trans.x), z.Dot(Trans.x), w.Dot(Trans.x)),
		Vector4(x.Dot(Trans.y), y.Dot(Trans.y), z.Dot(Trans.y), w.Dot(Trans.y)),
		Vector4(x.Dot(Trans.z), y.Dot(Trans.z), z.Dot(Trans.z), w.Dot(Trans.z)),
		Vector4(x.Dot(Trans.w), x.Dot(Trans.w), z.Dot(Trans.w), w.Dot(Trans.w)));
}
