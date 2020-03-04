#pragma once
#include "vec3.h"

class Vector4
{
public:
	float x, y, z, w;

	// Constructors
	Vector4();
	Vector4(float x, float y, float z, float w);
	Vector4(const Vector3& rhs, float w);
	Vector4(const Vector4& rhs);

	// Public API 
	Vector4 Abs() const;
	float Dot(const Vector4& b) const;
	float Length() const;
	float LengthSquared() const;
	Vector4 Lerp(const Vector4& b, const float t);
	Vector4 Slerp(const Vector4& b, const float t);
	Vector4 Normalized() const;
	bool IsNormalized() const;

	// Operators
	float& operator [] (int i);
	float operator [] (int i) const;
	Vector4 operator + (const Vector4& rhs) const;
	Vector4 operator - (const Vector4& rhs) const;
	bool operator == (const Vector4& rhs) const;
	bool operator != (const Vector4& rhs) const;

	float operator * (const Vector4& rhs) const; // Equivalent to dot product
	float operator / (const Vector4& rhs) const; // Inverse of dot product
	
	friend Vector4 operator * (float scalar, const Vector4& v);
	friend Vector4 operator * (const Vector4& v, float scalar);

	friend Vector4 operator / (const Vector4& v, float scalar);
};
