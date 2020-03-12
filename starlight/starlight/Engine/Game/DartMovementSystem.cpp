#include "DartMovementSystem.h"
#include "TransformComponent.h"

void DartMovementSystem::Update(float deltaTime)
{
	for (auto& CompTuple : Components)
	{
		TransformComponent* TransformComp = std::get<TransformComponent*>(CompTuple);
		VelocityComponent* VelocityComp = std::get<VelocityComponent*>(CompTuple);

		Transform& Data = TransformComp->Data;
		Data = Data.Translate(VelocityComp->Velocity * deltaTime);
	}
}
