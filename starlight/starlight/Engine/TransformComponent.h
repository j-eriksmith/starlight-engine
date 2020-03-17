#pragma once
#include "Component.h"
#include "String/StringHash.h"
#include "math/transform.h"

class TransformComponent;
using TransformComponentPtr = std::shared_ptr<TransformComponent>;

struct TransformComponent : public Component
{
	explicit TransformComponent()
		: Component(UniqueID),
		Data() {}
	TransformComponent(const Matrix3& inBasis, const Vector3& inOrigin)
		: Component(UniqueID),
		Data(inBasis, inOrigin) {}
	TransformComponent(const Vector3& inX, const Vector3& inY, const Vector3& inZ, const Vector3& inOrigin)
		: Component(UniqueID),
		Data(inX, inY, inZ, inOrigin) {}
	TransformComponent(const Vector3& inX, const Vector3& inY, const Vector3& inZ, const Vector3& inOrigin, TransformComponent* transformParent)
		: Component(UniqueID),
		Data(inX, inY, inZ, inOrigin, &transformParent->Data) {}
	TransformComponent(TransformComponent* transformParent)
		: Component(UniqueID),
		Data(&transformParent->Data) {}

	Transform Data;

	// Todo(jake): Please fix this hard-coded comp ID
	// Currently loading from an online generator...
	static constexpr ComponentID UniqueID = StringIdHash("TransformComponent");
	static unsigned int EngineMemoryID;
};
