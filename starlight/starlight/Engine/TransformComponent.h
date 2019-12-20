#pragma once
#include "Component.h"
#include "String/StringHasher.h"
#include "math/transform.h"

struct TransformComponent : public Component
{
	explicit TransformComponent()
		: Data() {}
	TransformComponent(const Vector3& inX, const Vector3& inY, const Vector3& inZ, const Vector3& inOrigin)
		: Data(inX, inY, inZ, inOrigin) {}
	TransformComponent(const Vector3& inX, const Vector3& inY, const Vector3& inZ, const Vector3& inOrigin, TransformComponent* transformParent)
		: Data(inX, inY, inZ, inOrigin, &transformParent->Data) {}
	TransformComponent(TransformComponent* transformParent)
		: Data(&transformParent->Data) {}

	Transform Data;

	// Todo(jake): Please fix this hard-coded comp ID
	// Currently loading from an online generator...
	static constexpr ComponentID UniqueID = 0x811c9dc5; /* HashString("TransformComponent"); */
	static unsigned int EngineMemoryID;
};
