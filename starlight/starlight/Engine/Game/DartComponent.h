#pragma once

#include "Component.h"
#include "String/StringHash.h"
#include "math/vec3.h"

class DartComponent : public Component
{
public:
	explicit DartComponent()
		: Component(UniqueID),
		Velocity(),
		Lifespan(10.f) {}

	DartComponent(const Vector3& initVelocity, const float& lifespan = 10.f)
		: Component(UniqueID),
		Velocity(initVelocity),
		Lifespan(lifespan) {}

	Vector3 Velocity;
	float Lifespan;

	static constexpr ComponentID UniqueID = StringIdHash("DartComponent");
	static unsigned int EngineMemoryID;
};
