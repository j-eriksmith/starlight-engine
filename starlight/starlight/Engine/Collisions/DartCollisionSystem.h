#pragma once
#include "System.h"
#include "Game/DartComponent.h"
#include "CollisionComponent.h"
#include "TransformComponent.h"

class DartCollisionSystem : public System<DartComponent, CollisionComponent, TransformComponent>
{
	using BaseType = System<DartComponent, CollisionComponent, TransformComponent>;
public:
	DartCollisionSystem()
		: BaseType(nullptr) {}
	DartCollisionSystem(Engine* pEntityEngine)
		: BaseType(pEntityEngine) {}

	virtual void Update(float deltaTime) override;
};