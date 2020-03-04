#pragma once
#include "Component.h"
#include "vec3.h"
#include "String/StringHash.h"

class CollisionComponent : public Component
{
public:
	explicit CollisionComponent()
		:Component(UniqueID),
		center(Vector3(0.0f, 0.0f, 0.0f)),
		radiusX(0.0f),
		radiusY(0.0f),
		radiusZ(0.0f)
	{}

	CollisionComponent(Vector3 c, float x, float y, float z)
		:Component(UniqueID),
		center(c),
		radiusX(x),
		radiusY(y),
		radiusZ(z)
	{}

	static constexpr ComponentID UniqueID = StringIdHash("CollisionComponent");
	static unsigned int EngineMemoryID;

private:
	Vector3 center;
	float radiusX;
	float radiusY;
	float radiusZ;
};