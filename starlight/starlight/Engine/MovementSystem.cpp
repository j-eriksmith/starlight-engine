#include "MovementSystem.h"
#include "TransformComponent.h"
#include "Engine.h"

MovementComponent* MovementSystem::GetMovementComponent( Vector3 v)
{
	return new (MemMgr::Alloc(sizeof MovementComponent, MemMgr::AllocatorType::LevelData)) MovementComponent(v);
}

void MovementSystem::Update(float deltaTime)
{
	//for (auto& CompTuple : Components)
	//{
	//	MovementComponent* mComp = std::get<MovementComponent*>(CompTuple);
	//	if (mComp->ShouldDestroy)
	//	{
	//		EntityEngine->DestroyEntity(mComp->OwningEntity);
	//	}
	//}
}

void MovementSystem::UpdateVelocity(MovementComponent* m, Vector3 newVelocity)
{
	m->Velocity = newVelocity;
}

void MovementSystem::TransferData(MovementComponent* src, MovementComponent* dst)
{
	dst->Velocity = src->Velocity;
}
