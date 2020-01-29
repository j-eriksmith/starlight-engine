#include "transform.h"

Transform::Transform()
	: Parent(nullptr)
{}

Transform::Transform(Transform* inParent)
	: Parent(inParent)
{}

Transform::Transform(const Vector3& inX, const Vector3& inY, const Vector3& inZ, const Vector3& inOrigin)
	: Basis(inX, inY, inZ),
	Origin(inOrigin),
	Parent(nullptr)
{}

Transform::Transform(const Vector3& inX, const Vector3& inY, const Vector3& inZ, const Vector3& inOrigin, Transform * inParent)
	: Basis(inX, inY, inZ),
	Origin(inOrigin),
	Parent(inParent)
{}

Transform::Transform(Matrix3 inBasis, Vector3 inOrigin)
	: Basis(inBasis),
	Origin(inOrigin),
	Parent(nullptr)
{}

Transform::Transform(Matrix3 inBasis, Vector3 inOrigin, Transform * inParent)
	: Basis(inBasis),
	Origin(inOrigin),
	Parent(inParent)
{}

Transform Transform::Rotate(Vector3 axis, float angle) const
{
	return Transform(Basis.Rotate(axis, angle), Origin, Parent);
}

Transform Transform::Scale(Vector3 scale) const
{
	return Transform(Basis.Scale(scale), Origin, Parent);
}

Transform Transform::Translate(Vector3 location) const
{
	return Transform(Basis, Origin + location, Parent);
}

Transform Transform::GetWorldTransform() const
{
	// As noted in my notebook, this approach will cause some unnecessary cache misses in the ECS loop because of pointer chasing to resolve the transform.
	Transform WorldTransform(*this);
	for (Transform* TransformCursor = GetParent(); TransformCursor != nullptr; TransformCursor = TransformCursor->GetParent())
	{
		WorldTransform.Basis = TransformCursor->Basis * WorldTransform.Basis;
		WorldTransform.Origin = TransformCursor->Origin + WorldTransform.Origin;
	}
	return WorldTransform;
}

bool Transform::operator==(const Transform& rhs)
{
	return Origin == rhs.Origin && Basis == rhs.Basis;
}
