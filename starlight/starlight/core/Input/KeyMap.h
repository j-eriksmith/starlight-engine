#pragma once

// Each key uses GLFW's values, just prettier names for our engine
enum class Keys
{
	SPACE = 32,
  	APOSTROPHE = 39, /* ' */
  	COMMA = 44, /* , */
	MINUS = 45, /* - */
	PERIOD = 46, /* . */
	SLASH = 47, /* / */
	NUM_0 = 48,
	NUM_1 = 49,
	NUM_2 = 50,
	NUM_3 = 51,
	NUM_4 = 52,
	NUM_5 = 53,
	NUM_6 = 54,
	NUM_7 = 55,
	NUM_8 = 56,
	NUM_9 = 57,
	SEMICOLON = 59, /* ; */
	EQUAL = 61, /* = */
	A = 65,
	B = 66,
	C = 67,
	D = 68,
	E = 69,
	F = 70,
	G = 71,
	H = 72,
	I = 73,
	J = 74,
	K = 75,
	L = 76,
	M = 77,
	N = 78,
	O = 79,
	P = 80,
	Q = 81,
	R = 82,
	S = 83,
	T = 84,
	U = 85,
	V = 86,
	W = 87,
	X = 88,
	Y = 89,
	Z = 90,
	LEFT_BRACKET = 91, /* [ */
	BACKSLASH = 92, /* \ */
	RIGHT_BRACKET = 93, /* ] */
	GRAVE_ACCENT = 96, /* ` */
	ESCAPE = 256,
	ENTER = 257,
	TAB = 258,
	BACKSPACE = 259,
	INSERT = 260,
	DELETE = 261,
	RIGHT_ARROW = 262,
	LEFT_ARROW = 263,
	DOWN_ARROW = 264,
	UP_ARROW = 265,
	PAGE_UP = 266,
	PAGE_DOWN = 267,
	HOME = 268,
	END = 269,
	CAPS_LOCK = 280,
	SCROLL_LOCK = 281,
	NUM_LOCK = 282,
	PRINT_SCREEN = 283,
	F1 = 290,
	F2 = 291,
	F3 = 292,
	F4 = 293,
	F5 = 294,
	F6 = 295,
	F7 = 296,
	F8 = 297,
	F9 = 298,
	F10 = 299,
	F11 = 300,
	F12 = 301,
	NUMPAD_0 = 320,
	NUMPAD_1 = 321,
	NUMPAD_2 = 322,
	NUMPAD_3 = 323,
	NUMPAD_4 = 324,
	NUMPAD_5 = 325,
	NUMPAD_6 = 326,
	NUMPAD_7 = 327,
	NUMPAD_8 = 328,
	NUMPAD_9 = 329,
	NUMPAD_DECIMAL = 330,
	NUMPAD_DIVIDE = 331,
	NUMPAD_MULTIPLY = 332,
	NUMPAD_SUBTRACT = 333,
	NUMPAD_ADD = 334,
	NUMPAD_ENTER = 335,
	NUMPAD_EQUAL = 336,
	LEFT_SHIFT = 340,
	LEFT_CONTROL = 341,
	LEFT_ALT = 342,
	LEFT_SUPER = 343, // Left Windows key
	RIGHT_SHIFT = 344,
	RIGHT_CONTROL = 345,
	RIGHT_ALT = 346,
	RIGHT_SUPER = 347, // Right Windows key
	LAST = 348
};

enum class MouseButton
{
	MOUSE_LEFT, // also MOUSE_BUTTON_1
	MOUSE_RIGHT, // also MOUSE_BUTTON_2
	MOUSE_MIDDLE, // also MOUSE_BUTTON_3
	MOUSE_4,
	MOUSE_5,
	MOUSE_6,
	MOUSE_7,
	MOUSE_8, // also MOUSE_BUTTON_LAST
	LAST
};

/*
static unsigned int StarlightToGLFW(Keys GLFWkey)
{
	switch (GLFWkey)
	{
	case Keys::SPACE:
		return 32;
	case Keys::APOSTROPHE:
		case 
	}
}
*/
