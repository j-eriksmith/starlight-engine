#include "DartMovementSystem.h"
#include "TransformComponent.h"
#include "Engine.h"

void DartMovementSystem::Update(float deltaTime)
{
	for (auto& CompTuple : Components)
	{
		TransformComponent* TransformComp = std::get<TransformComponent*>(CompTuple);
		DartComponent* DartComp = std::get<DartComponent*>(CompTuple);
		MovementComponent* MovementComp = std::get<MovementComponent*>(CompTuple);
		Transform& Data = TransformComp->Data;
		Log("&&&&&&&&&& DartComp Address: " << DartComp);
		Log("&&&&&&&&&& Dart Velocity is " << DartComp->Velocity.x << ", " << DartComp->Velocity.y << ", " << DartComp->Velocity.z);
		Data = Data.Translate(MovementComp->Velocity * deltaTime);
		
		DartComp->Lifespan -= deltaTime;

		//if (DartComp->Lifespan <= 0.f)
		//{
		//	EntityEngine->DestroyEntity(DartComp->OwningEntity);
		//}
	}
}

void DartMovementSystem::StopDart(DartComponentTuple* dart)
{
	Log("******* Entered DartMovementSystem::StopDart *********");
	DartComponent* DartComp = std::get<DartComponent*>(*dart);
	Log("******* DartComp address: " << DartComp);
	DartComp->Velocity = Vector3(0.0f, 0.0f, 0.0f);
}
