#pragma once
#include <SDL.h>

// Singleton class type design
class InputManager {
public:
	static InputManager* Get() {
		static InputManager manager;
		return &manager;
	}
	enum TexType {
		LEFT, RIGHT, MIDDLE, BACK, FORWARD
	};

	bool ProcessInput();
	void Shutdown();

	bool KeyDown(SDL_Scancode scancode);
	bool KeyPressed(SDL_Scancode scancode);
	bool KeyReleased(SDL_Scancode scancode);

private:
	InputManager();
	~InputManager();

	bool HandleEvent(SDL_Event* event);

	Uint8* m_prevKeyboardState;
	const Uint8* m_keyboardState;
	int m_keyLen;

	Uint32 m_prevMouseState;
	Uint32 m_mouseState;
};