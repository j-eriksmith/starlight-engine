#pragma once

// Each key uses GLFW's values, just prettier names for our engine
enum class Keys
{
	SPACE,
  	APOSTROPHE, /* ' */
  	COMMA, /* , */
	MINUS, /* - */
	PERIOD, /* . */
	SLASH, /* / */
	NUM_0,
	NUM_1,
	NUM_2,
	NUM_3,
	NUM_4,
	NUM_5,
	NUM_6,
	NUM_7,
	NUM_8,
	NUM_9,
	SEMICOLON, /* ; */
	EQUAL, /* = */
	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	LEFT_BRACKET, /* [ */
	BACKSLASH, /* \ */
	RIGHT_BRACKET, /* ] */
	GRAVE_ACCENT, /* ` */
	ESCAPE,
	ENTER,
	TAB,
	BACKSPACE,
	INSERT,
	DELETE,
	RIGHT_ARROW,
	LEFT_ARROW,
	DOWN_ARROW,
	UP_ARROW,
	PAGE_UP,
	PAGE_DOWN,
	HOME,
	END,
	CAPS_LOCK,
	SCROLL_LOCK,
	NUM_LOCK,
	PRINT_SCREEN,
	F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,
	NUMPAD_0,
	NUMPAD_1,
	NUMPAD_2,
	NUMPAD_3,
	NUMPAD_4,
	NUMPAD_5,
	NUMPAD_6,
	NUMPAD_7,
	NUMPAD_8,
	NUMPAD_9,
	NUMPAD_DECIMAL,
	NUMPAD_DIVIDE,
	NUMPAD_MULTIPLY,
	NUMPAD_SUBTRACT,
	NUMPAD_ADD,
	NUMPAD_ENTER,
	NUMPAD_EQUAL,
	LEFT_SHIFT,
	LEFT_CONTROL,
	LEFT_ALT,
	LEFT_SUPER, // Left Windows key
	RIGHT_SHIFT,
	RIGHT_CONTROL,
	RIGHT_ALT,
	RIGHT_SUPER, // Right Windows key
	COUNT
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
	COUNT	
};

inline static unsigned int StarlightToGLFW(Keys StarlightKey)
{
	switch (StarlightKey)
	{
	case Keys::SPACE:
		return 32;
	case Keys::APOSTROPHE:
		return 39;
	case Keys::COMMA:
	case Keys::MINUS:
	case Keys::PERIOD:
	case Keys::SLASH:
	case Keys::NUM_0:
	case Keys::NUM_1:
	case Keys::NUM_2:
	case Keys::NUM_3:
	case Keys::NUM_4:
	case Keys::NUM_5:
	case Keys::NUM_6:
	case Keys::NUM_7:
	case Keys::NUM_8:
	case Keys::NUM_9:
		return 42 + static_cast<unsigned int>(StarlightKey);
	case Keys::SEMICOLON:
		return 59;
	case Keys::EQUAL:
		return 61;
	case Keys::A:
	case Keys::B:
	case Keys::C:
	case Keys::D:
	case Keys::E:
	case Keys::F:
	case Keys::G:
	case Keys::H:
	case Keys::I:
	case Keys::J:
	case Keys::K:
	case Keys::L:
	case Keys::M:
	case Keys::N:
	case Keys::O:
	case Keys::P:
	case Keys::Q:
	case Keys::R:
	case Keys::S:
	case Keys::T:
	case Keys::U:
	case Keys::V:
	case Keys::W:
	case Keys::X:
	case Keys::Y:
	case Keys::Z:
	case Keys::LEFT_BRACKET:
	case Keys::BACKSLASH:
	case Keys::RIGHT_BRACKET:
		return 47 + static_cast<unsigned int>(StarlightKey);
	case Keys::GRAVE_ACCENT:
		return 96;
	case Keys::ESCAPE:
	case Keys::ENTER:
	case Keys::TAB:
	case Keys::BACKSPACE:
	case Keys::INSERT:
	case Keys::DELETE:
	case Keys::RIGHT_ARROW:
	case Keys::LEFT_ARROW:
	case Keys::DOWN_ARROW:
	case Keys::UP_ARROW:
	case Keys::PAGE_UP:
	case Keys::PAGE_DOWN:
	case Keys::HOME:
	case Keys::END:
		return 208 + static_cast<unsigned int>(StarlightKey);
	case Keys::CAPS_LOCK:
	case Keys::SCROLL_LOCK:
	case Keys::NUM_LOCK:
	case Keys::PRINT_SCREEN:
		return 218 + static_cast<unsigned int>(StarlightKey);
	case Keys::F1:
	case Keys::F2:
	case Keys::F3:
	case Keys::F4:
	case Keys::F5:
	case Keys::F6:
	case Keys::F7:
	case Keys::F8:
	case Keys::F9:
	case Keys::F10:
	case Keys::F11:
	case Keys::F12:
		return 224 + static_cast<unsigned int>(StarlightKey);
	case Keys::NUMPAD_0:
	case Keys::NUMPAD_1:
	case Keys::NUMPAD_2:
	case Keys::NUMPAD_3:
	case Keys::NUMPAD_4:
	case Keys::NUMPAD_5:
	case Keys::NUMPAD_6:
	case Keys::NUMPAD_7:
	case Keys::NUMPAD_8:
	case Keys::NUMPAD_9:
	case Keys::NUMPAD_DECIMAL:
	case Keys::NUMPAD_DIVIDE:
	case Keys::NUMPAD_MULTIPLY:
	case Keys::NUMPAD_SUBTRACT:
	case Keys::NUMPAD_ADD:
	case Keys::NUMPAD_ENTER:
	case Keys::NUMPAD_EQUAL:
		return 242 + static_cast<unsigned int>(StarlightKey);
	case Keys::LEFT_SHIFT:
	case Keys::LEFT_CONTROL:
	case Keys::LEFT_ALT:
	case Keys::LEFT_SUPER:
	case Keys::RIGHT_SHIFT:
	case Keys::RIGHT_CONTROL:
	case Keys::RIGHT_ALT:
	case Keys::RIGHT_SUPER:
		return 245 + static_cast<unsigned int>(StarlightKey);
	default:
		return 0;
	}
}
