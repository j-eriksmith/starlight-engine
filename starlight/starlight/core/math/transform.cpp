#include "transform.h"
#include "MemMgr.h"

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

Matrix4* Transform::GetMat4() const
{
	return reinterpret_cast<Matrix4*>(new (MemMgr::Alloc(sizeof(Matrix4), 
									  MemMgr::AllocatorType::FrameData)) 
									  Matrix4(Vector4(Basis.x.x, Basis.y.x, Basis.z.x, 0.0f),
									  Vector4(Basis.x.y, Basis.y.y, Basis.z.y, 0.0f),
									  Vector4(Basis.x.z, Basis.y.z, Basis.z.z, 0.0f),
									  Vector4(Origin.x, Origin.y, Origin.z, 1.0f)));
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
