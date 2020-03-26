#pragma once
#include "System.h"
#include "TransformComponent.h"
#include "TargetComponent.h"
#include "MovementComponent.h"

using TargetComponentTuple = std::tuple< TransformComponent*, TargetComponent*, MovementComponent*>;

class TargetMovementSystem : public System<TransformComponent, TargetComponent, MovementComponent>
{
	using BaseType = System<TransformComponent, TargetComponent, MovementComponent>;
public:
	TargetMovementSystem()
		: BaseType(nullptr) {}
	TargetMovementSystem(Engine* pEntityEngine)
		: BaseType(pEntityEngine) {}

	virtual void Update(float deltaTime) override;
};
