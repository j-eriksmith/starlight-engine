#include "vec4.h"
#include "utils.h"
#include <stdexcept>

Vector4::Vector4()
	: x(0.0f), 
	y(0.0f), 
	z(0.0f), 
	w(0.0f)
{}

Vector4::Vector4(float x, float y, float z, float w)
	: x(x),
	y(y), 
	z(z), 
	w(w) 
{}

Vector4::Vector4(const Vector4& rhs)
	: x(rhs.x),
	y(rhs.y),
	z(rhs.z),
	w(rhs.w)
{}

Vector4 Vector4::Abs() const
{
	return Vector4(Utils::Abs(x), Utils::Abs(y), Utils::Abs(z), Utils::Abs(w));
}

float Vector4::Dot(const Vector4& b) const
{
	return x * b.x + y * b.y + z * b.z + w * b.w;
}

float Vector4::Length() const
{
	return Utils::Sqrt(LengthSquared());
}

float Vector4::LengthSquared() const
{
	return *this * *this;
}

Vector4 Vector4::Lerp(const Vector4& b, const float t)
{
	return *this + t * (b - *this);
}

Vector4 Vector4::Slerp(const Vector4& b, const float t)
{
	// Implementation from Math Magician
	float DotProd = Dot(b);
	float Interpolated = Utils::Acos(DotProd) * t; // scaled angle between start and end
	Vector4 RelativeVec = (b - *this * DotProd).Normalized();

	return *this * Utils::Cos(Interpolated) + RelativeVec * Utils::Sin(Interpolated);
}

Vector4 Vector4::Normalized() const
{
	return *this / this->Length();
}

bool Vector4::IsNormalized() const
{
	// Todo: might need to include a soft error tolerance for floating point imprecision
	return this->LengthSquared() == 1.0f;
}

float& Vector4::operator[](int i)
{
	switch (i)
	{
		case 0: return x;
		case 1: return y;
		case 2: return z;
		case 3: return w;
		default: throw std::out_of_range("Vector4::[] out of bounds access.");
	}
}

float Vector4::operator[](int i) const
{
	switch (i)
	{
		case 0: return x;
		case 1: return y;
		case 2: return z;
		case 3: return w;
		default: throw std::out_of_range("Vector4::[] out of bounds access.");
	}
}

Vector4 Vector4::operator+(const Vector4& rhs) const
{
	return Vector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
}

Vector4 Vector4::operator-(const Vector4& rhs) const
{
	return Vector4(x - rhs.x, y - rhs.y, z - rhs.z, w + rhs.w);
}

bool Vector4::operator==(const Vector4& rhs) const
{
	float Epsilon = 0.1f;
	float Diff = x - rhs.x + y - rhs.y + z - rhs.z + w - rhs.w;
	return Utils::Abs(Diff) <= Epsilon;
}

bool Vector4::operator!=(const Vector4& rhs) const
{
	return !(*this == rhs);
}

float Vector4::operator*(const Vector4& rhs) const
{
	return Dot(rhs);
}

float Vector4::operator/(const Vector4& rhs) const
{
	return 1 / Dot(rhs);
}

Vector4 operator*(float scalar, const Vector4& v)
{
	return Vector4(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
}

Vector4 operator*(const Vector4& v, float scalar)
{
	return operator*(scalar, v);
}

Vector4 operator/(const Vector4& v, float scalar)
{
	return Vector4(v.x / scalar, v.y / scalar, v.z / scalar, v.w / scalar);
}
