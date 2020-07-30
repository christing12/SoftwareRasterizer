#include <SDL.h>
#include "InputManager.h"
#include <iostream>
#include "RenderManager.h"
#include "RenderObj.h"
#include "Scene.h"

InputManager::InputManager() {
	std::cout << "Input Manager being initted" << std::endl;
	m_keyboardState = SDL_GetKeyboardState(&m_keyLen);
	m_prevKeyboardState = new Uint8[m_keyLen];
	memcpy(m_prevKeyboardState, m_keyboardState, m_keyLen);
}

InputManager::~InputManager() {
	delete [] m_prevKeyboardState;
	m_prevKeyboardState = nullptr;
}

bool InputManager::KeyDown(SDL_Scancode scancode) {
	return m_keyboardState[scancode];
}


bool InputManager::KeyPressed(SDL_Scancode scancode) {
	return !m_prevKeyboardState[scancode] && KeyDown(scancode);
}

bool InputManager::KeyReleased(SDL_Scancode scancode) {
	return !m_keyboardState[scancode] && m_prevKeyboardState[scancode];
}

// checking between important engine level events and regular events
// NOTE: maybe combine handle even with this?
bool InputManager::ProcessInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			return false;
		}
		else {
			return HandleEvent(&event);
		}
		
	}
	return true;
}

void InputManager::Shutdown() {

}

// Handles individual key presses!
bool InputManager::HandleEvent(SDL_Event* event) {
	if (event->type == SDL_KEYDOWN) {
		switch (event->key.keysym.sym)
		{
		case SDLK_ESCAPE:
			std::cout << "Escaping!" << std::endl;
			return false;
		case SDLK_1:
			RenderManager* rm;
			rm = RenderManager::Get();
			rm->wireframe = !rm->wireframe;
			rm->Render();
			break;
		case SDLK_EQUALS:
			RenderObj* obj = RenderManager::Get()->GetCurrScene()->GetRenderObjs()[0];
			obj->rotSpeed = std::abs(obj->rotSpeed);
			RenderManager::Get()->Update(1.f);
			RenderManager::Get()->Render();
			break;
		}

	}
	return true;
}