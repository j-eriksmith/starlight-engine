#pragma once

// I have a feeling I should look into an inlining system
// then lots of things will be moved into that

class Vector3
{
public:
	float x, y, z;

	// Constructors
	Vector3();
	Vector3(float x, float y, float z);
	Vector3(const Vector3& rhs);

	// Public API 
	Vector3 Abs() const;
	Vector3 Cross(const Vector3& b) const;
	float Dot(const Vector3& b) const;
	float Length() const;
	float LengthSquared() const;
	Vector3 Lerp(const Vector3& b, const float t);
	Vector3 Slerp(const Vector3& b, const float t);
	Vector3 Project(const Vector3& b);
	Vector3 Reflect(const Vector3& n);
	Vector3 Normalized() const;
	bool IsNormalized() const;

	// Operators
	float& operator [] (int i);
	float operator [] (int i) const;
	Vector3 operator + (const Vector3& rhs) const;
	Vector3 operator - (const Vector3& rhs) const;
	bool operator == (const Vector3& rhs) const;
	bool operator != (const Vector3& rhs) const;

	float operator * (const Vector3& rhs) const; // Equivalent to dot product
	float operator / (const Vector3& rhs) const; // Inverse of dot product
	
	friend Vector3 operator * (float scalar, const Vector3& v);
	friend Vector3 operator * (const Vector3& v, float scalar);

	friend Vector3 operator / (const Vector3& v, float scalar);

	static void RunTests();
};
