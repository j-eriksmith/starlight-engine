#pragma once
#include "System.h"
#include "TransformComponent.h"

class MovementSystem : public System<TransformComponent>
{
	using BaseType = System<TransformComponent>;
public:
	MovementSystem()
		: BaseType(nullptr) {}
	MovementSystem(Engine* pEntityEngine)
		: BaseType(pEntityEngine) {}

	virtual void Update(float deltaTime) override;
};
