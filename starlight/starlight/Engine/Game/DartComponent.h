#pragma once

#include "Component.h"
#include "String/StringHash.h"
#include "math/vec3.h"

class DartComponent : public Component
{
public:
	explicit DartComponent()
		: Component(UniqueID),
		Lifespan(10.f) {}

	DartComponent(const float& lifespan)
		: Component(UniqueID),
		Lifespan(lifespan) {}

	float Lifespan;

	static constexpr ComponentID UniqueID = StringIdHash("DartComponent");
	static unsigned int EngineMemoryID;
};
