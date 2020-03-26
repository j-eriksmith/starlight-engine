#pragma once

#include "Component.h"
#include "String/StringHash.h"
#include "math/vec3.h"

class TargetComponent : public Component
{
public:
	explicit TargetComponent()
		: Component(UniqueID),
		InitTimeBeforeReverse(5.f),
		TimeBeforeReverse(InitTimeBeforeReverse) {}

	TargetComponent(const Vector3& initVelocity, const float& timeBeforeReverse = 10.f)
		: Component(UniqueID),
		InitTimeBeforeReverse(timeBeforeReverse),
		TimeBeforeReverse(timeBeforeReverse) {}

	float InitTimeBeforeReverse;
	float TimeBeforeReverse;

	static constexpr ComponentID UniqueID = StringIdHash("TargetComponent");
	static unsigned int EngineMemoryID;
};
