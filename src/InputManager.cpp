#include <SDL.h>
#include "InputManager.h"
#include <iostream>
#include "RenderManager.h"
#include "RenderObj.h"
#include "Scene.h"

InputManager::InputManager() {
	std::cout << "Input Manager being initted" << std::endl;

	// --- keyboard initialize -- //
	m_keyboardState = SDL_GetKeyboardState(&m_keyLen);
	m_prevKeyboardState = new Uint8[m_keyLen];
	memcpy(m_prevKeyboardState, m_keyboardState, m_keyLen);

	m_mouseState = SDL_GetMouseState(&m_mouseXPos, &m_mouseYPos);
}

InputManager::~InputManager() {
	delete [] m_prevKeyboardState;
	m_prevKeyboardState = nullptr;
}

void InputManager::Update(float deltaTime) {
	m_mouseState = SDL_GetMouseState(&m_mouseXPos, &m_mouseYPos);
	m_mouseWheelY = 0;
}

void InputManager::UpdatePrevInput() {
	memcpy(m_prevKeyboardState, m_keyboardState, m_keyLen);
	m_prevMouseState = m_mouseState;
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

// Top left is the origin
Vector2 InputManager::MousePos() {
	return Vector2(float(m_mouseXPos), float(m_mouseYPos));
}

int InputManager::GetMouseMask(MOUSE_BUTTON button) {
	Uint32 mask = 0;
	switch (button) {
	case LEFT:
		mask = SDL_BUTTON_LMASK;
		break;
	case RIGHT:
		mask = SDL_BUTTON_RMASK;
		break;
	case MIDDLE:
		mask = SDL_BUTTON_MMASK;
		break;
	case BACK:
		mask = SDL_BUTTON_X1MASK;
		break;
	case FORWARD:
		mask = SDL_BUTTON_X2MASK;
		break;
	}
	return mask;
}

// returns true if button is pressed this frame
bool InputManager::MouseButtonDown(MOUSE_BUTTON button) {
	Uint32 mask = GetMouseMask(button);
	return (m_mouseState & mask);
}

// returns true if button was not pressed last frame but pressed this frame
bool InputManager::MouseButtonPressed(MOUSE_BUTTON button) {
	Uint32 mask = GetMouseMask(button);
	return !(m_prevMouseState & mask) && (m_mouseState & mask);
}

// return true if the button was pressed 
bool InputManager::MouseButtonReleased(MOUSE_BUTTON button) {
	Uint32 mask = GetMouseMask(button);
	return (m_prevMouseState & mask) && !(m_mouseState & mask);
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
	else if(event->type == SDL_MOUSEWHEEL) {
		Sint32 temp = event->wheel.y;
		if (temp > 0) m_mouseWheelY = 1;
		else if (temp < 0) m_mouseWheelY = -1;
	}
	return true;
}