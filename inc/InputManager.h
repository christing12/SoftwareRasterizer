#pragma once
#include <SDL.h>
#include "EngineMath.h"


enum MOUSE_BUTTON {
	LEFT, RIGHT, MIDDLE, BACK, FORWARD
};

// Singleton class type design
class InputManager {
public:
	InputManager();
	~InputManager();

	// --- Input Controls --- //
	void Update(float deltaTime);
	void UpdatePrevInput();
	bool ProcessInput();
	void Shutdown();

	// --- Keyboard Methods --- //
	bool KeyDown(SDL_Scancode scancode);
	bool KeyPressed(SDL_Scancode scancode);
	bool KeyReleased(SDL_Scancode scancode);

	// --- Mouse Button Methods --- //
	bool MouseButtonDown(MOUSE_BUTTON button);
	bool MouseButtonPressed(MOUSE_BUTTON button);
	bool MouseButtonReleased(MOUSE_BUTTON button);
	Vector2 MousePos();

	int WheelPos() { return m_mouseWheelY; }


private:


	bool HandleEvent(SDL_Event* event);

	// -- Keyboard State Info --- //
	Uint8* m_prevKeyboardState;
	const Uint8* m_keyboardState;
	int m_keyLen;

	// -- Mouse State Info --- //
	Uint32 m_prevMouseState;
	Uint32 m_mouseState;
	int m_mouseXPos;
	int m_mouseYPos;
	int m_mouseWheelY;


	int GetMouseMask(MOUSE_BUTTON button);
	void OnMouseWheelScroll(SDL_MouseWheelEvent* wheelEvent);
};