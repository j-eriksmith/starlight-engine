#include "ThrowDartSystem.h"
#include "Input/Input.h"
#include "Audio/AudioEngine.h"
#include "TransformComponent.h"
#include "ShaderComponent.h"
#include "RenderableComponent.h"
#include "CollisionComponent.h"
#include "VelocityComponent.h"
#include "CollisionSystem.h"
#include "ModelLoadingSystem.h"
#include "ShaderSystem.h"
#include "RenderingSystem.h"
#include "Camera.h"
#include "Engine.h"

void ThrowDartSystem::Update(float deltaTime)
{
	static ShaderComponentPtr dartShader(ShaderSystem::CreateShaderComponent("core/RenderingAPI/res/shaders/Basic.shader"));
	static RenderableComponentPtr targetRenderable(ModelLoadingSystem::LoadModel("core/RenderingAPI/res/models/dart/dart.obj"));
	static CollisionComponentPtr DartCollisionComp(CollisionSystem::GetCollisionComponent(targetRenderable));

	TransformComponent model(Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f));
	if (Input::GetKeyDown(Keys::SPACE))
	{
		glm::vec3 cameraPos = EntityEngine->CameraPtr->cameraPos;
		Vector3 cameraLookAt(EntityEngine->CameraPtr->viewTarget.x, EntityEngine->CameraPtr->viewTarget.y, EntityEngine->CameraPtr->viewTarget.z);
		Vector3 cameraUp(EntityEngine->CameraPtr->upVector.x, EntityEngine->CameraPtr->upVector.y, EntityEngine->CameraPtr->upVector.z);

		Entity* Dart = EntityEngine->CreateEntity();
		//glm::mat4 matrix = glm::lookAt(cameraPos, cameraPos + EntityEngine->CameraPtr->viewTarget, EntityEngine->CameraPtr->upVector);
		//Matrix3 RotationBasis(Vector3(matrix[0].x, matrix[0].y, matrix[0].z), Vector3(matrix[1].x, matrix[1].y, matrix[1].z), Vector3(matrix[2].x, matrix[2].y, matrix[2].z));
		TransformComponent DartTransform(Matrix3(), Vector3(cameraPos.x, cameraPos.y, cameraPos.z));
		DartTransform.Data = DartTransform.Data.Scale(Vector3(0.02f, 0.02f, 0.02f));
		Dart->AddComponent<TransformComponent>()->Data = DartTransform.Data;

		Dart->AddComponent<VelocityComponent>()->Velocity = cameraLookAt * 15.0f;
		ShaderSystem::TransferData(dartShader.get(), Dart->AddComponent<ShaderComponent>());
		RenderingSystem::TransferData(targetRenderable.get(), Dart->AddComponent<RenderableComponent>());
		CollisionSystem::TransferData(DartCollisionComp.get(), Dart->AddComponent<CollisionComponent>());
			
		AudioEngine::PlaySound("Resources/Audio/handleCoins.ogg");
	}
}
