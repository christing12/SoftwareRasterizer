#include "stdafx.h"
#include "DisplayManager.h"
#include <iostream>
#include <stdlib.h>
#include <cstring>

// Separate from constructor in case of restart
bool DisplayManager::Init() {
	std::cout << " Display Manager being initted lol" << std::endl; 

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
		return false;
	}

	m_window = SDL_CreateWindow("Software Rasterizer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	if (m_window == nullptr) {
		std::cout << "Issue with creating window: " << SDL_GetError() << std::endl;
		return false;
	}

	m_surface = SDL_GetWindowSurface(m_window);
	if (m_surface == nullptr) {
		std::cout << "Issue with getting surface: " << SDL_GetError() << std::endl;
		return false;
	}
	return true;
}

// Separate from destructor in case of restart
void DisplayManager::Shutdown() {
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

// Swaps the front buffer witht the backbuffer and displays it
void DisplayManager::UpdateDisplay(Buffer<Uint32>* buffer) {
	SDL_LockSurface(m_surface);

	memcpy(m_surface->pixels, buffer->buffer, buffer->height * buffer->stride);

	SDL_UnlockSurface(m_surface);
	SDL_UpdateWindowSurface(m_window);
}