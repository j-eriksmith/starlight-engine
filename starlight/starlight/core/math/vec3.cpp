#include "vec3.h"
#include "utils.h"
#include <stdexcept>

Vector3::Vector3()
	: x(0.0f),
	y(0.0f), 
	z(0.0f) 
{}

Vector3::Vector3(float x, float y, float z)
	: x(x),
	y(y),
	z(z) 
{}

Vector3::Vector3(const Vector3 & rhs)
	: x(rhs.x),
	y(rhs.y),
	z(rhs.z) 
{}

Vector3 Vector3::Abs() const
{
	return Vector3(Utils::Abs(x), Utils::Abs(y), Utils::Abs(z));
}

Vector3 Vector3::Cross(const Vector3 & b) const
{
	return Vector3(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);
}

float Vector3::Dot(const Vector3 & b) const
{
	return x * b.x + y * b.y + z * b.z;
}

float Vector3::Length() const
{
	return Utils::Sqrt(LengthSquared());
}

float Vector3::LengthSquared() const
{
	return *this * *this;
}

Vector3 Vector3::Lerp(const Vector3 & b, const float t)
{
	return *this + t * (b - *this);
}

Vector3 Vector3::Slerp(const Vector3 & b, const float t)
{
	// Implementation from Math Magician
	// Pretty sure it doesn't work
	float dot = Dot(b);
	float Interpolated = Utils::Acos(dot) * t; // scaled angle between start and end
	Vector3 RelativeVec = (b - *this * dot).Normalized();

	return *this * Utils::Cos(Interpolated) + RelativeVec * Utils::Sin(Interpolated);
}

Vector3 Vector3::Project(const Vector3& b)
{
	Vector3 bNormalized = b.Normalized();
	return Dot(bNormalized) * bNormalized;
}

Vector3 Vector3::Reflect(const Vector3 & n)
{
	if (!n.IsNormalized());// TODO: yell at them
	
	return *this - (2 * Dot(n)) * n;
}

Vector3 Vector3::Normalized() const
{
	return *this / this->Length();
}

bool Vector3::IsNormalized() const
{
	// Todo: might need to include a soft error tolerance for floating point imprecision
	return this->LengthSquared() == 1.0f;
}

float & Vector3::operator[](const int& i)
{
	switch (i)
	{
		case 0: return x;
		case 1: return y;
		case 2: return z;
		default: throw std::out_of_range("Vector3::[] out of bounds access.");
	}
}

float Vector3::operator[](const int& i) const
{
	switch (i)
	{
		case 0: return x;
		case 1: return y;
		case 2: return z;
		default: throw std::out_of_range("Vector3::[] out of bounds access.");
	}
}

Vector3 Vector3::operator+(const Vector3 & rhs) const
{
	return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
}

Vector3 Vector3::operator-(const Vector3 & rhs) const
{
	return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
}

bool Vector3::operator==(const Vector3& rhs) const
{
	float Epsilon = 0.1f;
	float Diff = x - rhs.x + y - rhs.y + z - rhs.z;
	return Utils::Abs(Diff) <= Epsilon;
}

bool Vector3::operator!=(const Vector3& rhs) const
{
	return !(*this == rhs);
}

float Vector3::operator*(const Vector3 & rhs) const
{
	return Dot(rhs);
}

float Vector3::operator/(const Vector3 & rhs) const
{
	return 1 / Dot(rhs);
}

Vector3 operator*(const float& scalar, const Vector3 & v)
{
	return Vector3(v.x * scalar, v.y * scalar, v.z * scalar);
}

Vector3 operator*(const Vector3 & v, const float& scalar)
{
	return operator*(scalar, v);
}

Vector3 operator/(const Vector3 & v, const float& scalar)
{
	return Vector3(v.x / scalar, v.y / scalar, v.z / scalar);
}
