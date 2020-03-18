#include "TargetMovementSystem.h"
#include "Engine.h"

void TargetMovementSystem::Update(float deltaTime)
{	
	for (auto& CompTuple : Components)
	{
		TargetComponent* TargetComp = std::get<TargetComponent*>(CompTuple);
		MovementComponent* MovementComp = std::get<MovementComponent*>(CompTuple);
		Transform& Data = std::get<TransformComponent*>(CompTuple)->Data;
		Data = Data.Translate(MovementComp->Velocity * deltaTime);
		TargetComp->TimeBeforeReverse -= deltaTime;
		if (TargetComp->TimeBeforeReverse <= 0.0f)
		{
			MovementComp->Velocity = MovementComp->Velocity * -1.0f;
			TargetComp->TimeBeforeReverse = TargetComp->InitTimeBeforeReverse;
		}
	}
}
