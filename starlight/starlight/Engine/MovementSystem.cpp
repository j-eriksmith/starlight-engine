#include "MovementSystem.h"
#include "TransformComponent.h"

void MovementSystem::Update(float deltaTime)
{
	for (auto& CompTuple : Components)
	{
		TransformComponent* TransformComp = std::get<TransformComponent*>(CompTuple);
		Transform& Data = TransformComp->Data;
		Data = Data.Translate(Vector3(100, 100, 100));

		// Testing parented transforms
		Transform WorldTransform = Data.GetWorldTransform();
	}
}
