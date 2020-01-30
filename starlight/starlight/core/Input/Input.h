#pragma once
#include "GLFWModule.h"
#include "KeyMap.h"

class Input
{
public:
	// Returns true only once on the frame that the key was pressed
	// [in]
	// Keys: Starlight key to query
	// [out]
	// Value of key down state for this frame
	static bool GetKeyDown(const Keys key);

	// Returns true for every frame that the key remains pressed
	// [in]
	// Keys: Starlight key to query
	// [out]
	// Value of current key state for this frame
	static bool GetKey(const Keys key);

	// Returns true only once on the frame that the key was released
	// [in]
	// Keys: Starlight key to query
	// [out]
	// Value of key up state for this frame
	static bool GetKeyUp(const Keys key);

	// Returns true only once on the frame that the mouse button was pressed
	// [in]
	// Keys: Starlight mouse button to query
	// [out]
	// Value of mouse button down state for this frame
	static bool GetMouseDown(const MouseButton mouseButton);

	// Returns true for every frame that the mouse button remains pressed
	// [in]
	// Keys: Starlight mouse button to query
	// [out]
	// Value of current mouse button state for this frame
	static bool GetMouse(const MouseButton mouseButton);

	// Returns true only once on the frame that the mouse button was released
	// [in]
	// Keys: Starlight key to query
	// [out]
	// Value of mouse button up state for this frame
	static bool GetMouseUp(const MouseButton mouseButton);

	// Returns current mouse position in pixels relative to the upper-left corner of the engine window
	// [in/out] 
	// x: x-value of the mouse position
	// y: y-value of the mouse position
	static void GetMousePosition(double& x, double& y);

	static void Initialize(GLFWwindow* pWindow);

private:
	static void Update();
	static GLFWModule Module; 

	friend class Engine;
};
