#include "matrix3.h"
#include <iostream>
#include "utils.h"

Matrix3::Matrix3()
	: x(Vector3(1.0f, 0.0f, 0.0f)),
		y(Vector3(0.0f, 1.0f, 0.0f)),
		z(Vector3(0.0f, 0.0f, 1.0f)) 
{}

Matrix3::Matrix3(const Vector3& v)
	: x(v.x, 0.0f, 0.0f),
	y(0.0f, v.y, 0.0f),
	z(0.0f, 0.0f, v.z)
{}

Matrix3::Matrix3(const Vector3& inX, const Vector3& inY, const Vector3& inZ)
	: x(inX),
		y(inY),
		z(inZ)
{}

float Matrix3::Determinant() const
{
	return x[0] * (y[1] * z[2] - y[2] * z[1])
		- y[0] * (x[1] * z[2] - x[2] * z[1])
		+ z[0] * (x[1] * y[2] - x[2] * y[1]);
}

Vector3 Matrix3::GetEulerAngle() const
{
	// Implementation from Learn OpenCV
	float sy = Utils::Sqrt(x.x * x.x + x.y * x.y);
	bool singular = sy < 1e-6;
	
	float angleX, angleY, angleZ;
	if (!singular)
	{
		angleX = Utils::Atan2(z.y, z.z);
		angleY = Utils::Atan2(z.x, sy);
		angleZ = Utils::Atan2(y.x, x.x);
	}
	else
	{
		angleX = Utils::Atan2(-y.z, y.y);
		angleY = Utils::Atan2(z.x, sy);
		angleZ = 0.f;
	}
	return Vector3(angleX, angleY, angleZ);
}

Vector3 Matrix3::GetScale() const
{
	return Vector3(x.x, y.y, z.z);
}

Matrix3 Matrix3::Inverse() const
{
	float D = Determinant();
	if (D == 0)
	{
		throw std::out_of_range("Matrix3::Inverse => Cannot do inverse when the determinant is zero.");
	}

	return 1/D * Matrix3(Vector3((y[1] * z[2]) - (y[2] * z[1]),
						z[1] * x[2] - z[2] * x[1],
						x[1] * y[2] - x[2] * y[1]),
				Vector3(z[0] * y[2] - z[2] * y[0],
						x[0] * z[2] - x[2] * z[0],
						y[0] * x[2] - y[2] * x[0]),
				Vector3(y[0] * z[1] - y[1] * z[0],
						z[0] * x[1] - z[1] * x[0],
						x[0] * y[1] - x[1] * y[0]));
}

Matrix3 Matrix3::Transpose() const
{
	return Matrix3(Vector3(x[0], y[0], z[0]), Vector3(x[1], y[1], z[1]), Vector3(x[2], y[2], z[2]));
}

Matrix3 Matrix3::Scale(const Vector3& scalar) const
{
	// Cheating with no multiplication since we know we're doing a scale operation
	return Matrix3(Vector3(x.x * scalar.x, x.y, x.z),
					Vector3(y.x, y.y * scalar.y, y.z),
					Vector3(z.x, z.y, z.z * scalar.z));
}

Matrix3 Matrix3::Rotate(const Vector3& axis, const float& angle) const
{
	// Not checking for normalization at this level - let developer fail
	// Implementation: wikipedia rotation matrix from axis and angle

	float Sin = Utils::Sin(angle);
	float Cos = Utils::Cos(angle);
	Matrix3 Rotation(Vector3(Cos + axis.x * axis.x * (1 - Cos), 
							axis.x * axis.y * (1 - Cos) - axis.z * Sin,
							axis.x * axis.y * (1 - Cos) - axis.y * Sin),
					Vector3(axis.y * axis.x * (1- Cos) - axis.z * Sin,
							Cos + axis.y * axis.y * (1 - Cos),
							axis.y * axis.z * (1 - Cos) - axis.x * Sin),
					Vector3(axis.z * axis.x * (1 - Cos) - axis.y * Sin,
							axis.z * axis.y * (1 - Cos) + axis.x * Sin,
							Cos + axis.z * axis.z * (1 - Cos)));
	return Rotation * *this;
}

Matrix3 operator*(const float& scalar, const Matrix3& m)
{
	return Matrix3(m.x * scalar, m.y * scalar, m.z * scalar);
}

Matrix3 operator*(const Matrix3& m, const float& scalar)
{
	return Matrix3(m.x * scalar, m.y * scalar, m.z * scalar);
}

bool Matrix3::operator==(const Matrix3& rhs)
{
	float Epsilon = 0.00001f;
	float Diff = x.x - rhs.x.x + x.y - rhs.x.y + x.z - rhs.x.z
		+ y.x - rhs.y.x + y.y - rhs.y.y + y.z - rhs.y.z
		+ z.x - rhs.z.x + z.y - rhs.z.y + z.z - rhs.z.z;
	return Utils::Abs(Diff) <= Epsilon;
}

Matrix3 Matrix3::operator*(const Matrix3& rhs)
{
	Matrix3 Trans(rhs.Transpose());

	return Matrix3(Vector3(x.Dot(Trans.x), y.Dot(Trans.x), z.Dot(Trans.x)),
					Vector3(x.Dot(Trans.y), y.Dot(Trans.y), z.Dot(Trans.y)),
					Vector3(x.Dot(Trans.z), y.Dot(Trans.z), z.Dot(Trans.z)));
}
