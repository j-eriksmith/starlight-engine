#pragma once
#include "Component.h"
#include "vec3.h"
#include "String/StringHash.h"



class MovementComponent : public Component
{
public:

	explicit MovementComponent()
		:Component(UniqueID),
		Velocity(Vector3(0.f,0.f,0.f))
	{}

	MovementComponent
	(
		Vector3 v
	)
		:Component(UniqueID),
		Velocity(v)
	{}

	Vector3 Velocity;
	static constexpr ComponentID UniqueID = StringIdHash("MovementComponent");
	static unsigned int EngineMemoryID;
};