#pragma once
#include "System.h"
#include "TransformComponent.h"
#include "MovementComponent.h"
#include "MemMgr/MemMgr.h"

using MovementComponentPtr = std::shared_ptr<MovementComponent>;

class MovementSystem : public System<MovementComponent>
{
	using BaseType = System<MovementComponent>;
public:
	MovementSystem()
		: BaseType(nullptr) {}
	MovementSystem(Engine* pEntityEngine)
		: BaseType(pEntityEngine) {}

	static MovementComponent* GetMovementComponent( Vector3 v);

	virtual void Update(float deltaTime) override;

	static void UpdateVelocity(MovementComponent* m, Vector3 v);

	static void TransferData(MovementComponent* src, MovementComponent* dst);
};
