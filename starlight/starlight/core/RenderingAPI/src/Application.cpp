#include <GLAD/include/glad.h>
#include <GLFW/include/glfw3.h>
#include <assimp/include/assimp/scene.h>
#include <assimp/include/assimp/Importer.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include "BoundingBox.h"
#include "Camera.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "transform.h"
#include "TransformComponent.h"
#include "Window.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Input/Input.h"
#include "Audio/AudioEngine.h"
#include "ResourceMgr/Resources.h"
#include "Debug.h"
#include "MemMgr/MemMgr.h"
#include "Time/Clock.h"
#include "math/vec3.h"

// ECS Includes
#include "CollisionComponent.h"
#include "CollisionSystem.h"
#include "RenderableComponent.h"
#include "ShaderComponent.h"
#include "TransformComponent.h"
#include "ModelLoadingSystem.h"
#include "RenderingSystem.h"
#include "ShaderSystem.h"
#include "Engine/Engine.h"
#include "Entity.h"

constexpr auto screenHeight = 960;
constexpr auto screenWidth = 960;
const double pi = 3.14159265358979323846;

float degToRad(float deg)
{
	return deg * (pi / 180.0f);
}

int main(void)
{
	std::shared_ptr<Window> window(new Window(screenHeight, screenWidth));

	// BEGIN ENGINE STARTUP

	//ResourceMgr rm;
	//std::thread ioThread(FileIO::WaitForRequests);
	MemMgr::Create(100 * 1048576); // 100MB
	Clock startupClock; // Engine timekeeping
	Engine e; // Initializes test data for Engine via its function InitTest
	//e.AddSystem<RenderingSystem>();
	//e.EnableSystem<RenderingSystem>();
	//e.AddSystem<ShaderSystem>();
	//e.EnableSystem<ShaderSystem>();
	//e.AddSystem<ModelLoadingSystem>();
	//e.EnableSystem<ModelLoadingSystem>();
	AudioEngine::Initialize(50 * 1048576); // 50MB
	AudioEngine::LoadSound(Resources::Get("Audio/music.mp3"), true);
	AudioEngine::LoadSound(Resources::Get("Audio/handleCoins.ogg"), false, false, true);
	Input::Initialize(window->GetWindow());

	double LastLoopTime = startupClock.GetTimeSinceStartup();
	double AccumulatedLag = 0.0;
	constexpr float S_PER_UPDATE = 0.0167f; // 60 fps

	// END ENGINE STARTUP

	// binds our shader
	//GLCall(Shader modelShader("core/RenderingAPI/res/shaders/Basic.shader"));

	// Represents the source coordinate of our lamp's light
	glm::vec3 lightPos(1.0, 1.0, -3.0);
	// 2/20/2020 - Look into RenderableComponent* and ShaderComponent* linker errors
	// 3/4/2020 TODO: Uncomment GL data structure destructors and switch data members to smart ptrs
	RenderableComponentPtr cy(ModelLoadingSystem::LoadModel("core/RenderingAPI/res/models/cylinder/cylinder.fbx"));
	ShaderComponentPtr modelShader(ShaderSystem::CreateShaderComponent("core/RenderingAPI/res/shaders/Basic.shader"));
	TransformComponent model(Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f));
	CollisionComponentPtr cc(CollisionSystem::GetCollisionComponent(cy));
	//model.Data = model.Data.Scale(Vector3(0.2f, 0.2f, 0.2f));
	Entity* cylinder = e.CreateEntity();
	RenderableComponent* cR = cylinder->AddComponent<RenderableComponent>();
	ShaderComponent* cS = cylinder->AddComponent<ShaderComponent>();
	Log("ShaderComponent Memory Address: " << reinterpret_cast<uintptr_t>(cS));
	TransformComponent* cT = cylinder->AddComponent<TransformComponent>();
	CollisionComponent* cC = cylinder->AddComponent<CollisionComponent>();
	RenderingSystem::TransferData(cy.get(), cR);
	cT->Data = model.Data;
	ShaderSystem::TransferData(modelShader.get(), cS);
	CollisionSystem::TransferData(cc.get(), cC);
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();

	 // Setup Platform/Renderer bindings
	const char* glsl_version = "#version 150";
	ImGui_ImplGlfw_InitForOpenGL(window->GetWindow(), true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	Camera::CreateCameraContext(window->GetWindow());
	std::shared_ptr<Camera> Cam(Camera::CreateCamera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

	//std::shared_ptr<MeshModel> m(new MeshModel("core/RenderingAPI/res/models/fbx/grasscube.fbx"));
	//std::shared_ptr<MeshModel> m(new MeshModel("core/RenderingAPI/res/models/fbx/eyeball.fbx"));
	//std::shared_ptr<MeshModel> m(new MeshModel("core/RenderingAPI/res/models/bullseye/target.obj"));
	std::shared_ptr<MeshModel> m(new MeshModel("core/RenderingAPI/res/models/dart/dart.obj"));
	//std::shared_ptr<Model> m(new MeshModel("core/RenderingAPI/res/models/crysis-nano-suit-2/textures/scene.fbx"));
	// std::shared_ptr<BoundingBox> mBox(m->GetBoundingBox());
	// std::shared_ptr<Model> m1(new MeshModel("core/RenderingAPI/res/models/crysis-nano-suit-2/textures/scene.fbx"));
	// std::shared_ptr<Model> sphere(new MeshModel("core/RenderingAPI/res/models/sphere/sphere.fbx"));
	//std::shared_ptr<MeshModel> cylinder(new MeshModel("core/RenderingAPI/res/models/cylinder/cylinder.fbx"));
	//std::shared_ptr<BoundingBox> cBox(cylinder->GetBoundingBox());
	//std::shared_ptr<Model> defaultModel(new DefaultModel(ShapeLoader::ShapeType::Cube, Resources::Get("grass_texture.jpg")));
	float offset = .01;
	int direction = 1;
	float x = 0.0;
	while (!window->ShouldClose())
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		RenderingSystem::Clear();

		Cam->ProcessInput();

		double CurrentLoopTime = startupClock.GetTimeSinceStartup();
		double DeltaTime =  CurrentLoopTime - LastLoopTime;
		LastLoopTime = CurrentLoopTime;
		AccumulatedLag += DeltaTime;
		while (AccumulatedLag >= S_PER_UPDATE)
		{
			e.Update(S_PER_UPDATE);
			AccumulatedLag -= S_PER_UPDATE;
		}

		static float xDelta = 0.0f;
		static float yDelta = 0.0f;
		static float r = 0.0f;

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window->
		{
			static int counter = 0;
			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("floatX", &xDelta, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)& clear_color); // Edit 3 floats representing a color
			ImGui::SliderFloat("floatY", &yDelta, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::SliderFloat("rotate", &r, 1.0f, 360.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		//glm::mat4 model = glm::mat4(1.0f);
		//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::scale(model, glm::vec3(0.001f, 0.001f, 0.001f));
		model = glm::scale(model, glm::vec3(.02f, .02f, .02f));

		glm::mat4 view = glm::mat4(1.0f);
		Cam->ApplyViewMatrix(view);

		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);

		//glm::mat4 mvp = projection * view * model;
		//Vector3 ones(1.0f, 1.0f, 1.0f);
		//Vector3 zeros(0.0f, 0.0f, 0.0f);
		//TransformComponent transform(ones, ones, ones, zeros);


		/*
		lightingShader.Bind();
		lightingShader.SetUniformMat4f("model", model);
		lightingShader.SetUniformMat4f("view", view);
		lightingShader.SetUniformMat4f("projection", projection);
		lightingShader.SetUniformMat4f("u_MVP", mvp);
		lightingShader.SetUniform3f("viewPos", Cam->cameraPos.x, Cam->cameraPos.y, Cam->cameraPos.z);
		lightingShader.Unbind();
		*/
		
		//model = glm::translate(model, lightPos);
		//model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
		//lampShader.Bind();
		//lampShader.SetUniformMat4f("model", model);
		//lampShader.SetUniformMat4f("view", view);
		//lampShader.SetUniformMat4f("projection", projection);
		//lampShader.SetUniformMat4f("u_MVP", mvp);
		//defaultModel->Draw(lampShader);
		//model = glm::translate(model, glm::vec3(1.0, 1.0, 1.0));
		//lampShader.SetUniformMat4f("model", model);
		//defaultModel->Draw(lampShader);
		//lampShader.Unbind();

		//modelShader.Bind();
		//modelShader.SetUniformMat4f("model", model);
		//modelShader.SetUniformMat4f("view", view);
		//modelShader.SetUniformMat4f("projection", projection);
		//nanosuit->Draw(modelShader);
		//modelShader.Unbind();

		//modelShader.Bind();
		modelShader.Bind();
		//if (x > 5)
		//	direction = -1;
		//else if (x < 0)
		//	direction = 1;
		//float newOffset = offset * direction;
		//x += newOffset;
		//cBox->UpdateCenter(newOffset, 0.0, 0.0);
		//modelShader.Bind();
		//modelShader.SetUniform4f("u_Color", 0.0, 1.0, 0.0, 1.0);
		//modelShader.SetUniformMat4f("model", model);
		//modelShader.SetUniformMat4f("view", view);
		//modelShader.SetUniformMat4f("projection", projection);
		// modelShader.SetUniform4f("u_Color", 1.0, 0.0, 0.0, 1.0);
		modelShader.SetUniformMat4f("model", model);
		modelShader.SetUniformMat4f("view", view);
		modelShader.SetUniformMat4f("projection", projection);
		//
		//if (cBox->HasCollided(*mBox))
		//{
		//	modelShader.SetUniform4f("u_Color", 1.0, 0.0, 0.0, 1.0);
		//}
		//m->Draw(modelShader);
		m->Draw(modelShader);
		//model = glm::translate(model, glm::vec3(x, 0.0, 0.0));
		//modelShader.SetUniformMat4f("model", model);
		//modelShader.SetUniformMat4f("view", view);
		//modelShader.SetUniformMat4f("projection", projection);
		//cylinder->Draw(modelShader);
		//modelShader.Unbind();
		modelShader.Unbind();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		
		AudioEngine::Update({ Cam->cameraPos.x, Cam->cameraPos.y, Cam->cameraPos.z }, { Cam->viewTarget.x, Cam->viewTarget.y, Cam->viewTarget.z });
		window->EndFrame();
	}

	// Cleanup
	//ImGui_ImplOpenGL3_Shutdown();
	//ImGui_ImplGlfw_Shutdown();
	//ImGui::DestroyContext();
	AudioEngine::Shutdown();

	return 0;
}