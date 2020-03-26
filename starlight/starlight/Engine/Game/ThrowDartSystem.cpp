#include "ThrowDartSystem.h"
#include "Input/Input.h"
#include "Audio/AudioEngine.h"
#include "TransformComponent.h"
#include "ShaderComponent.h"
#include "RenderableComponent.h"
#include "CollisionComponent.h"
#include "DartComponent.h"
#include "CollisionSystem.h"
#include "ModelLoadingSystem.h"
#include "MovementSystem.h"
#include "ShaderSystem.h"
#include "RenderingSystem.h"
#include "Camera.h"
#include "Engine.h"

void ThrowDartSystem::Update(float deltaTime)
{
	static ShaderComponentPtr dartShader(ShaderSystem::CreateShaderComponent("core/RenderingAPI/res/shaders/Basic.shader"));
	static RenderableComponentPtr targetRenderable(ModelLoadingSystem::LoadModel("core/RenderingAPI/res/models/dart/dart.obj"));
	static CollisionComponent* DartCollisionComp(CollisionSystem::GetCollisionComponent(targetRenderable));
	DartCollisionComp->collidableType = CollisionComponent::CollidableType::Dart;
	CollisionSystem::Scale(DartCollisionComp, Vector3(0.02f, 0.02f, 0.02f));

	TransformComponent model(Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f));
	if (Input::GetKeyDown(Keys::SPACE))
	{
		glm::vec3 cameraPos = EntityEngine->CameraPtr->cameraPos;
		Vector3 cameraLookAt(EntityEngine->CameraPtr->viewTarget.x, EntityEngine->CameraPtr->viewTarget.y, EntityEngine->CameraPtr->viewTarget.z);
		Vector3 cameraUp(EntityEngine->CameraPtr->upVector.x, EntityEngine->CameraPtr->upVector.y, EntityEngine->CameraPtr->upVector.z);
		
		// Dart Instantiation
		Entity* Dart = EntityEngine->CreateEntity();

		TransformComponent DartTransform(Matrix3(), Vector3(cameraPos.x, cameraPos.y, cameraPos.z));
		DartTransform.Data = DartTransform.Data.Scale(Vector3(0.02f, 0.02f, 0.02f));
		Dart->AddComponent<TransformComponent>()->Data = DartTransform.Data;

		DartComponent* DartComp = Dart->AddComponent<DartComponent>();
  		MovementComponent* MovementComponentComp (MovementSystem::GetMovementComponent(cameraLookAt * 25.0f));
		DartComp->Lifespan = 2.f;
		ShaderSystem::TransferData(dartShader.get(), Dart->AddComponent<ShaderComponent>());
		RenderingSystem::TransferData(targetRenderable.get(), Dart->AddComponent<RenderableComponent>());
		CollisionSystem::TransferData(DartCollisionComp, Dart->AddComponent<CollisionComponent>());
		MovementSystem::TransferData(MovementComponentComp, Dart->AddComponent<MovementComponent>());
		AudioEngine::PlaySound("Resources/Audio/throwdart.wav");
	}
}
