#pragma once
#include "System.h"
#include "TransformComponent.h"
#include "VelocityComponent.h"

class DartMovementSystem : public System<TransformComponent, VelocityComponent>
{
	using BaseType = System<TransformComponent, VelocityComponent>;
public:
	DartMovementSystem()
		: BaseType(nullptr) {}
	DartMovementSystem(Engine* pEntityEngine)
		: BaseType(pEntityEngine) {}

	virtual void Update(float deltaTime) override;
};
