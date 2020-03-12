#include "ThrowDartSystem.h"
#include "Input/Input.h"
#include "Audio/AudioEngine.h"
#include "TransformComponent.h"
#include "ShaderComponent.h"
#include "RenderableComponent.h"
#include "ModelLoadingSystem.h"
#include "ShaderSystem.h"
#include "RenderingSystem.h"
#include "Camera.h"
#include "Engine.h"

void ThrowDartSystem::Update(float deltaTime)
{
	static ShaderComponentPtr modelShader(ShaderSystem::CreateShaderComponent("core/RenderingAPI/res/shaders/Basic.shader"));
	static RenderableComponentPtr cy(ModelLoadingSystem::LoadModel("core/RenderingAPI/res/models/cylinder/cylinder.fbx"));

	TransformComponent model(Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f));
	if (Input::GetKeyDown(Keys::SPACE))
	{
		glm::vec3 cameraPos = EntityEngine->CameraPtr->cameraPos;

		Entity* Dart = EntityEngine->CreateEntity();
		TransformComponent DartTransform(Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(cameraPos.x, cameraPos.y, cameraPos.z));
		Dart->AddComponent<TransformComponent>()->Data = DartTransform.Data;
		ShaderSystem::TransferData(modelShader.get(), Dart->AddComponent<ShaderComponent>());
		RenderingSystem::TransferData(cy.get(), Dart->AddComponent<RenderableComponent>());
		
		AudioEngine::PlaySound("Resources/Audio/handleCoins.ogg");
	}
}
