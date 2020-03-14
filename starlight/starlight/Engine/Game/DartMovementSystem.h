#pragma once
#include "System.h"
#include "TransformComponent.h"
#include "DartComponent.h"

class DartMovementSystem : public System<TransformComponent, DartComponent>
{
	using BaseType = System<TransformComponent, DartComponent>;
public:
	DartMovementSystem()
		: BaseType(nullptr) {}
	DartMovementSystem(Engine* pEntityEngine)
		: BaseType(pEntityEngine) {}

	virtual void Update(float deltaTime) override;
};
