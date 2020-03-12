#pragma once

#include "Component.h"
#include "String/StringHash.h"
#include "math/vec3.h"

class VelocityComponent : public Component
{
public:
	explicit VelocityComponent()
		: Component(UniqueID),
		Velocity() {}

	VelocityComponent(const Vector3& initVelocity)
		: Component(UniqueID),
		Velocity(initVelocity) {}

	Vector3 Velocity;

	static constexpr ComponentID UniqueID = StringIdHash("VelocityComponent");
	static unsigned int EngineMemoryID;
};
