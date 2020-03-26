#pragma once
#include "System.h"
#include "TransformComponent.h"
#include "DartComponent.h"
#include "MovementComponent.h"

using DartComponentTuple = std::tuple< TransformComponent*, DartComponent*, MovementComponent*>;
class DartMovementSystem : public System<TransformComponent, DartComponent, MovementComponent>
{
	using BaseType = System<TransformComponent, DartComponent, MovementComponent>;
public:
	DartMovementSystem()
		: BaseType(nullptr) {}
	DartMovementSystem(Engine* pEntityEngine)
		: BaseType(pEntityEngine) {}

	virtual void Update(float deltaTime) override;

	static void StopDart(DartComponentTuple* dart);
};
