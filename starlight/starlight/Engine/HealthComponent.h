#pragma once
#include "Component.h"

class HealthComponent : public Component
{
public:
	explicit HealthComponent()
		: Component(),
		CurrentHealth(0.0f),
		MaxHealth(0.0f) {}
	float CurrentHealth;
	float MaxHealth;

	static constexpr ComponentID UniqueID = 0x2e2135ca;
	static unsigned int EngineMemoryID;
};
