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
	// Do I make three temporary matricies so my fingers don't fall off?
	return x[0] * (y[1] * z[2] - y[2] * z[1])
		- y[0] * (x[1] * z[2] - x[2] * z[1])
		+ z[0] * (x[1] * y[2] - x[2] * y[1]);
}

Vector4 Matrix4::GetScale() const
{
	return Vector4(x.x, y.y, z.z, w.w);
}

Matrix4 Matrix4::Inverse() const
{
	float D = Determinant();
	if (D == 0)
	{
		// TODO: Error logging for attempting to invert this
		return Matrix4();
	}

	return 1/D * Matrix4(Vector4((y[1] * z[2]) - (y[2] * z[1]),
						z[1] * x[2] - z[2] * x[1],
						x[1] * y[2] - x[2] * y[1]),
				Vector4(z[0] * y[2] - z[2] * y[0],
						x[0] * z[2] - x[2] * z[0],
						y[0] * x[2] - y[2] * x[0]),
				Vector4(y[0] * z[1] - y[1] * z[0],
						z[0] * x[1] - z[1] * x[0],
						x[0] * y[1] - x[1] * y[0]));
}

Matrix4 Matrix4::Transpose() const
{
	return Matrix4(Vector4(x[0], y[0], z[0]), Vector4(x[1], y[1], z[1]), Vector4(x[2], y[2], z[2]));
}

Matrix4 Matrix4::Scale(const Vector4& scalar) const
{
	// Cheating with no multiplication since we know we're doing a scale operation
	return Matrix4(Vector4(x.x * scalar.x, x.y, x.z),
					Vector4(y.x, y.y * scalar.y, y.z),
					Vector4(z.x, z.y, z.z * scalar.z));
}

Matrix4 Matrix4::Rotate(Vector4 axis, float angle) const
{
	// Not checking for normalization at this level - let developer fail
	// Implementation: wikipedia rotation matrix from axis and angle

	float Sin = Utils::Sin(angle);
	float Cos = Utils::Cos(angle);
	Matrix4 Rotation(Vector4(Cos + axis.x * axis.x * (1 - Cos), 
							axis.x * axis.y * (1 - Cos) - axis.z * Sin,
							axis.x * axis.y * (1 - Cos) - axis.y * Sin),
					Vector4(axis.y * axis.x * (1- Cos) - axis.z * Sin,
							Cos + axis.y * axis.y * (1 - Cos),
							axis.y * axis.z * (1 - Cos) - axis.x * Sin),
					Vector4(axis.z * axis.x * (1 - Cos) - axis.y * Sin,
							axis.z * axis.y * (1 - Cos) + axis.x * Sin,
							Cos + axis.z * axis.z * (1 - Cos)));
	return Rotation * *this;
}

Matrix4 operator*(float scalar, Matrix4 m)
{
	return Matrix4(m.x * scalar, m.y * scalar, m.z * scalar);
}

Matrix4 operator*(Matrix4 m, float scalar)
{
	return Matrix4(m.x * scalar, m.y * scalar, m.z * scalar);
}

bool Matrix4::operator==(const Matrix4& rhs)
{
	float Epsilon = 0.00001f;
	float Diff = x.x - rhs.x.x + x.y - rhs.x.y + x.z - rhs.x.z
		+ y.x - rhs.y.x + y.y - rhs.y.y + y.z - rhs.y.z
		+ z.x - rhs.z.x + z.y - rhs.z.y + z.z - rhs.z.z;
	return Utils::Abs(Diff) <= Epsilon;
}

Matrix4 Matrix4::operator*(const Matrix4& rhs)
{
	Matrix4 Trans(rhs.Transpose());

	return Matrix4(Vector4(x.Dot(Trans.x), y.Dot(Trans.x), z.Dot(Trans.x)),
					Vector4(x.Dot(Trans.y), y.Dot(Trans.y), z.Dot(Trans.y)),
					Vector4(x.Dot(Trans.z), y.Dot(Trans.z), z.Dot(Trans.z)));
}
