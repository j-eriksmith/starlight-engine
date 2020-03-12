#pragma once
#include "System.h"
#include "PlayerComponent.h"

class ThrowDartSystem : public System<PlayerComponent>
{
	using BaseType = System<PlayerComponent>;
public:
	ThrowDartSystem()
		: BaseType(nullptr) {}
	ThrowDartSystem(Engine* pEntityEngine)
		: BaseType(pEntityEngine) {}

	virtual void Update(float deltaTime) override;
};
