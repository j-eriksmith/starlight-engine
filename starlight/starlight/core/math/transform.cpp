#include "transform.h"

Transform::Transform(Vector3 inX, Vector3 inY, Vector3 inZ, Vector3 inOrigin)
	: Basis(inX, inY, inZ),
		Origin(inOrigin)
{}

Transform::Transform(Matrix3 inBasis, Vector3 inOrigin)
	: Basis(inBasis),
		Origin(inOrigin)
{}

Transform Transform::Rotate(Vector3 axis, float angle)
{
	return Transform(Basis.Rotate(axis, angle), Origin);
}

Transform Transform::Scale(Vector3 scale)
{
	return Transform(Basis.Scale(scale), Origin);
}

Transform Transform::Translate(Vector3 location)
{
	return Transform(Basis, Origin + location);
}

bool Transform::operator==(const Transform& rhs)
{
	return Origin == rhs.Origin && Basis == rhs.Basis;
}
