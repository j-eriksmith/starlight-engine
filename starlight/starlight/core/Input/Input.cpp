#include "Input.h"

GLFWModule Input::Module{}; // Window member must be initialized later via Initialize(window)

bool Input::GetKeyDown(const Keys key)
{
	return Module.GetKeyDown(static_cast<unsigned int>(key));
}

bool Input::GetKey(const Keys key)
{
	return Module.GetKey(static_cast<unsigned int>(key));
}

bool Input::GetKeyUp(const Keys key)
{
	return Module.GetKeyUp(static_cast<unsigned int>(key));
}

bool Input::GetMouseDown(const MouseButton mouseButton)
{
	return Module.GetMouseDown(static_cast<unsigned int>(mouseButton));
}

bool Input::GetMouse(const MouseButton mouseButton)
{
	return Module.GetMouse(static_cast<unsigned int>(mouseButton));
}

bool Input::GetMouseUp(const MouseButton mouseButton)
{
	return Module.GetMouseUp(static_cast<unsigned int>(mouseButton));
}

void Input::GetMousePosition(double& x, double& y)
{
	Module.GetMousePosition(x, y);
}

void Input::Initialize(GLFWwindow* pWindow)
{
	Module.Initialize(pWindow);
}

void Input::Update()
{
	Module.Update();
}
