#include "Engine.h"
#include "DisplayManager.h"
#include "InputManager.h"
#include <iostream>
#include "Renderer.h"
#include "Mesh.h"
#include "Buffer.h"
#include "RenderManager.h"
#include <chrono>

#include <iostream>
// Separate from the constructor in case the engine needs to restart
bool Engine::Init() {
	bool successful = true;
	g_displayManager = DisplayManager::Get();
	if (!g_displayManager->Init()) {
		std::cout << "Error with Diplay Manager " << std::endl;
		return false;
	}

	g_inputManager = InputManager::Get();
	
	g_renderManager = RenderManager::Get();
	if (!g_renderManager->Init(DisplayManager::SCREEN_WIDTH, DisplayManager::SCREEN_HEIGHT)) {
		std::cout << "Error with Renderer" << std::endl;
		return false;
	}
	g_renderManager->Render();
	return true;
}

// main loop
// TODO: figure out calculating deltaTime and fps
void Engine::Run() {
	std::chrono::high_resolution_clock::time_point frameStart = std::chrono::high_resolution_clock::now();
	float deltaTime = 1.f / 60.f;
	while (isRunning) {
		std::chrono::high_resolution_clock::time_point frameEnd;
		do
		{
			frameEnd = std::chrono::high_resolution_clock::now();
			double duration = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(frameEnd - frameStart).count();
			deltaTime = (float)(0.000000001 * duration);
		} while (deltaTime < 0.9f / 60.0f);     // giving it a 10% buffer
		frameStart = frameEnd;

		g_inputManager->Update(deltaTime);
		isRunning = g_inputManager->ProcessInput();

		g_renderManager->Update(deltaTime);
		g_inputManager->UpdatePrevInput();

		g_renderManager->Render();

	}
	Shutdown();
}

// Separate from the destructor in case engine needs to restart(?)
void Engine::Shutdown() {
	g_displayManager->Shutdown();
	g_inputManager->Shutdown();
}