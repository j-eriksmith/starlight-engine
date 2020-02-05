#pragma once
#include "Component.h"
#include "String/StringHash.h"

class HealthComponent : public Component
{
public:
	explicit HealthComponent()
		: Component(),
		CurrentHealth(0.0f),
		MaxHealth(0.0f) {}
	float CurrentHealth;
	float MaxHealth;

	static constexpr ComponentID UniqueID = StringIdHash("HealthComponent");
	static unsigned int EngineMemoryID;
};
