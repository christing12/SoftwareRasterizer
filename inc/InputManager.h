#pragma once
#include <SDL.h>
#include "EngineMath.h"

// Singleton class type design
class InputManager {
public:
	static InputManager* Get() {
		static InputManager manager;
		return &manager;
	}
	enum MOUSE_BUTTON {
		LEFT, RIGHT, MIDDLE, BACK, FORWARD
	};

	void Update(float deltaTime);
	void UpdatePrevInput();
	bool ProcessInput();
	void Shutdown();

	bool KeyDown(SDL_Scancode scancode);
	bool KeyPressed(SDL_Scancode scancode);
	bool KeyReleased(SDL_Scancode scancode);

	bool MouseButtonDown(MOUSE_BUTTON button);
	bool MouseButtonPressed(MOUSE_BUTTON button);
	bool MouseButtonReleased(MOUSE_BUTTON button);

	Vector2 MousePos();

private:
	InputManager();
	~InputManager();

	bool HandleEvent(SDL_Event* event);

	Uint8* m_prevKeyboardState;
	const Uint8* m_keyboardState;
	int m_keyLen;

	Uint32 m_prevMouseState;
	Uint32 m_mouseState;

	int m_mouseXPos;
	int m_mouseYPos;

	int GetMouseMask(MOUSE_BUTTON button);
};