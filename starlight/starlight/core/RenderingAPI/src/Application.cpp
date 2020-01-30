#include <GLAD/include/glad.h>
#include <GLFW/include/glfw3.h>
#include <assimp/include/assimp/scene.h>
#include <assimp/include/assimp/Importer.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include "Model/MeshModel.h"
#include "Model/DefaultModel.h"
#include "Model/ShapeLoader.h"
#include "Camera.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
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
#include "Engine/Engine.h"

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

	AudioEngine::Initialize(50 * 1048576); // 50MB
	AudioEngine::LoadSound(Resources::Get("Audio/music.mp3"), true);
	AudioEngine::LoadSound(Resources::Get("Audio/handleCoins.ogg"), false, false, true);
	Input::Initialize(window->GetWindow());

	double LastLoopTime = startupClock.GetTimeSinceStartup();
	double AccumulatedLag = 0.0;
	constexpr float S_PER_UPDATE = 0.0167f; // 60 fps

	// END ENGINE STARTUP

	// binds our shader
	GLCall(Shader modelShader("core/RenderingAPI/res/shaders/Basic.shader"));

	// Represents the source coordinate of our lamp's light
	glm::vec3 lightPos(1.0, 1.0, -3.0);

	GLCall(Shader lightingShader("core/RenderingAPI/res/shaders/Lighting.shader"));
	lightingShader.Bind();
	lightingShader.SetUniform3f("lightColor", 1.0f, 1.0f, 1.0f);
	lightingShader.SetUniform3f("objectColor", 1.0f, 0.0f, 1.0f);
	lightingShader.SetUniform3f("lightPos", lightPos.x, lightPos.y, lightPos.z);
	lightingShader.Unbind();

	GLCall(Shader lampShader("core/RenderingAPI/res/shaders/Lamp.shader"));
	lampShader.Bind();
	lampShader.SetUniform3f("lightColor", 1.0f, 1.0f, 1.0f);
	lampShader.SetUniform3f("lightPos", lightPos.x, lightPos.y, lightPos.z);
	lampShader.Unbind();

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

	std::shared_ptr<Model> m(new MeshModel("core/RenderingAPI/res/models/crysis-nano-suit-2/textures/scene.fbx"));
	std::shared_ptr<Model> defaultModel(new DefaultModel(ShapeLoader::ShapeType::Cube));

	int IR = -1;
	while (!window->ShouldClose())
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		Renderer::Clear();

		Cam->ProcessInput();

		// BEGIN JAKE ENGINE PASTERINO
		double CurrentLoopTime = startupClock.GetTimeSinceStartup();
		double DeltaTime =  CurrentLoopTime - LastLoopTime;
		LastLoopTime = CurrentLoopTime;
		AccumulatedLag += DeltaTime;

		while (AccumulatedLag >= S_PER_UPDATE)
		{
			e.Update(S_PER_UPDATE);
			AccumulatedLag -= S_PER_UPDATE;
		}

		// END JAKE ENGINE PASTERINO
		if (Input::GetKeyDown(Keys::LEFT_SHIFT))
		{
			Log("I pressed Left Shift!");
			AudioEngine::PlaySound(Resources::Get("Audio/handleCoins.ogg"));
		}
		if (Input::GetKeyDown(Keys::F9))
		{
			Log("I pressed F9");
			AudioEngine::SetChannelPaused(IR, true);
		}
		if (Input::GetKeyDown(Keys::T))
		{
			Log("I pressed T!");
		}
		if (Input::GetKeyUp(Keys::T))
		{
			Log("I picked my finger off of T!");
		}
		if (Input::GetMouseDown(MouseButton::MOUSE_LEFT))
		{
			Log("I pressed down on LMB!");
			IR = AudioEngine::PlaySound(Resources::Get("Audio/music.mp3"), { 10.f, 0.f, 0.f });
		}
		if (Input::GetMouse(MouseButton::MOUSE_LEFT))
		{
			Log("I'm holding on LMB!");
		}
		if (Input::GetMouseUp(MouseButton::MOUSE_LEFT))
		{
			Log("I released LMB!");
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

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));

		glm::mat4 view = glm::mat4(1.0f);
		Cam->ApplyViewMatrix(view);

		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);

		glm::mat4 mvp = projection * view * model;

		lightingShader.Bind();
		lightingShader.SetUniformMat4f("model", model);
		lightingShader.SetUniformMat4f("view", view);
		lightingShader.SetUniformMat4f("projection", projection);
		lightingShader.SetUniformMat4f("u_MVP", mvp);
		lightingShader.SetUniform3f("viewPos", Cam->cameraPos.x, Cam->cameraPos.y, Cam->cameraPos.z);
		defaultModel->Draw(lightingShader);
		lightingShader.Unbind();
		
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
		lampShader.Bind();
		lampShader.SetUniformMat4f("model", model);
		lampShader.SetUniformMat4f("view", view);
		lampShader.SetUniformMat4f("projection", projection);
		lampShader.SetUniformMat4f("u_MVP", mvp);
		defaultModel->Draw(lampShader);
		model = glm::translate(model, glm::vec3(1.0, 1.0, 1.0));
		lampShader.SetUniformMat4f("model", model);
		defaultModel->Draw(lampShader);
		lampShader.Unbind();

		modelShader.Bind();
		lightingShader.SetUniformMat4f("model", model);
		lightingShader.SetUniformMat4f("view", view);
		lightingShader.SetUniformMat4f("projection", projection);
		m->Draw(modelShader);
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