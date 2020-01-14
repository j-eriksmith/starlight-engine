#include "GLFWModule.h"

void GLFWModule::Initialize(GLFWwindow* pInputWindow)
{
	InputWindow = pInputWindow;
}

void GLFWModule::Update()
{
	glfwPollEvents();
	SwapStateBuffers(PreviousKeyStates, CurrentKeyStates);
	SwapStateBuffers(PreviousMouseButtonStates, CurrentMouseButtonStates);
	UpdateCurrentStates();
}

bool GLFWModule::GetKey(const unsigned int requestedKey) const
{
	return CurrentKeyStates[requestedKey] == GLFW_PRESS;
}

bool GLFWModule::GetKeyDown(const unsigned int requestedKey) const
{ 
	return CurrentKeyStates[requestedKey] == GLFW_PRESS
		&& PreviousKeyStates[requestedKey] == GLFW_RELEASE;
}

bool GLFWModule::GetKeyUp(const unsigned int requestedKey) const
{
	return CurrentKeyStates[requestedKey] == GLFW_RELEASE
		&& PreviousKeyStates[requestedKey] == GLFW_PRESS;
}

bool GLFWModule::GetMouse(const unsigned int requestedMouseButton) const
{
	return CurrentMouseButtonStates[requestedMouseButton] == GLFW_PRESS;
}

bool GLFWModule::GetMouseDown(const unsigned int requestedMouseButton) const
{
	return CurrentMouseButtonStates[requestedMouseButton] == GLFW_PRESS
		&& PreviousMouseButtonStates[requestedMouseButton] == GLFW_RELEASE;
}

bool GLFWModule::GetMouseUp(const unsigned int requestedMouseButton) const
{
	return CurrentMouseButtonStates[requestedMouseButton] == GLFW_RELEASE
		&& PreviousMouseButtonStates[requestedMouseButton] == GLFW_PRESS;
}

void GLFWModule::GetMousePosition(double& x, double& y) const
{
	glfwGetCursorPos(InputWindow, &x, &y);
}

void GLFWModule::SwapStateBuffers(int*& prevStates, int*& currentStates)
{
	int* TwoFramesAgoStates = prevStates;
	prevStates = currentStates;
	currentStates = TwoFramesAgoStates;
}

void GLFWModule::UpdateCurrentStates()
{
	size_t KeyStatesSize = static_cast<size_t>(Keys::LAST);
	size_t MouseButtonStatesSize = static_cast<size_t>(MouseButton::LAST);
	for (size_t StatesIndex = 0; StatesIndex < KeyStatesSize; ++StatesIndex)
	{
		// Update Mouse Button States
		if (StatesIndex < MouseButtonStatesSize)
		{
			CurrentMouseButtonStates[StatesIndex] = glfwGetMouseButton(InputWindow, StatesIndex);
		}

		// Update Key States
		CurrentKeyStates[StatesIndex] = glfwGetKey(InputWindow, StatesIndex);
	}
}
