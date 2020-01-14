#pragma once

#include "GLFW/include/glfw3.h"
#include "KeyMap.h"

/*
	Backend for Input's polling scheme. Exists in a singleton instance owned by the Input class.
	Interfaces with GLFW so that client programmers don't have to!
*/

class GLFWModule
{
	GLFWModule() :
		InputWindow(nullptr),
		PreviousKeyStates(new int[static_cast<unsigned int>(Keys::LAST)]), // new'd because need pointers vs [] to swap buffers
		CurrentKeyStates(new int[static_cast<unsigned int>(Keys::LAST)]), // but only done once at startup
		PreviousMouseButtonStates(new int[static_cast<unsigned int>(MouseButton::LAST)]),
		CurrentMouseButtonStates(new int[static_cast<unsigned int>(MouseButton::LAST)])
	{}

	void Initialize(GLFWwindow* pInputWindow);
	void Update();
	bool GetKey(const unsigned int requestedKey) const;
	bool GetKeyDown(const unsigned int requestedKey) const;
	bool GetKeyUp(const unsigned int requestedKey) const;
	bool GetMouse(const unsigned int requestedMouseButton) const;
	bool GetMouseDown(const unsigned int requestedMouseButton) const;
	bool GetMouseUp(const unsigned int requestedMouseButton) const;
	void GetMousePosition(double& x, double& y) const;

	void SwapStateBuffers(int*& prevStates, int*& currentStates);
	void UpdateCurrentStates();

	GLFWwindow* InputWindow;
	int* PreviousKeyStates; 
	int* CurrentKeyStates;
	int* PreviousMouseButtonStates; 
	int* CurrentMouseButtonStates; 

	friend class Input;
};
