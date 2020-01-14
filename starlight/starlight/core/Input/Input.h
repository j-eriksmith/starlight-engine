#pragma once
#include "GLFWModule.h"
#include "KeyMap.h"

class Input
{
public:
	static bool GetKeyDown(const Keys key);
	static bool GetKey(const Keys key);
	static bool GetKeyUp(const Keys key);
	static bool GetMouseDown(const MouseButton mouseButton);
	static bool GetMouse(const MouseButton mouseButton);
	static bool GetMouseUp(const MouseButton mouseButton);
	static void GetMousePosition(double& x, double& y);

	// Todo(jake): When we have one application entry point at Engine, make these private.
	static void Initialize(GLFWwindow* pWindow);
	static void Update();

private:
	static GLFWModule Module; 

	friend class Engine;
};
