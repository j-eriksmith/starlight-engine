#include "DartMovementSystem.h"
#include "TransformComponent.h"
#include "Engine.h"

void DartMovementSystem::Update(float deltaTime)
{
	for (auto& CompTuple : Components)
	{
		TransformComponent* TransformComp = std::get<TransformComponent*>(CompTuple);
		DartComponent* DartComp = std::get<DartComponent*>(CompTuple);

		Transform& Data = TransformComp->Data;
		Data = Data.Translate(DartComp->Velocity * deltaTime);
		DartComp->Lifespan -= deltaTime;

		if (DartComp->Lifespan <= 0.f)
		{
			EntityEngine->DestroyEntity(DartComp->OwningEntity);
		}
	}
}
