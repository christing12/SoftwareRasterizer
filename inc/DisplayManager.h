#pragma once
#include <SDL.h>
#include "Buffer.h"

// Singleton type design
class DisplayManager {
public:
	DisplayManager() {}
	~DisplayManager() = default;
	const static int SCREEN_WIDTH = 800;
	const static int SCREEN_HEIGHT = 800;

	bool Init();
	void Shutdown();


	void UpdateDisplay(Buffer<Uint32>* buffer);
	SDL_PixelFormat* GetPixelFormat() { return m_surface->format; }

private:
	SDL_Window* m_window;
	SDL_Surface* m_surface;
};