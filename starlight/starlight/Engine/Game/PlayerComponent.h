#pragma once
#include "Component.h"
#include "String/StringHash.h"

class PlayerComponent : public Component
{
public:
	explicit PlayerComponent()
		: Component(UniqueID),
		CurrentAmmo(0),
		MaxAmmo(10) {}
	int CurrentAmmo;
	int MaxAmmo;

	static constexpr ComponentID UniqueID = StringIdHash("PlayerComponent");
	static unsigned int EngineMemoryID;
};
