#pragma once
#include "vec3.h"
#include "matrix3.h"

class Transform
{
public:
	Vector3 Origin;
	Matrix3	Basis;

	explicit Transform();
	Transform(Transform* inParent);
	Transform(const Vector3& inX, const Vector3& inY, const Vector3& inZ, const Vector3& inOrigin);
	Transform(const Vector3& inX, const Vector3& inY, const Vector3& inZ, const Vector3& inOrigin, Transform* inParent);
	Transform(Matrix3 inBasis, Vector3 inOrigin);
	Transform(Matrix3 inBasis, Vector3 inOrigin, Transform* inParent);

	Transform Rotate(Vector3 axis, float angle) const;
	Transform Scale(Vector3 scale) const;
	Transform Translate(Vector3 location) const;
	Transform* GetParent() const { return Parent; }
	void SetParent(Transform* newParent) { Parent = newParent; }
	Transform GetWorldTransform() const;

	bool operator==(const Transform& rhs);

	static void RunTests();

private:
	Transform* Parent;
};