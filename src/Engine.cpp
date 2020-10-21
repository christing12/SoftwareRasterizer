#include "stdafx.h"
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

	g_DisplayManager = CreateRef<DisplayManager>();
	if (!g_DisplayManager->Init()) {
		std::cout << "Error with Diplay Manager " << std::endl;
		return false;
	}

	g_InputManager = CreateRef<InputManager>();

	g_RenderManager = CreateRef<RenderManager>(g_DisplayManager, g_InputManager);
	if (!g_RenderManager->Init(DisplayManager::SCREEN_WIDTH, DisplayManager::SCREEN_HEIGHT)) {
		std::cout << "Error with Renderer" << std::endl;
		return false;
	}
	g_RenderManager->Render();
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

		g_InputManager->Update(deltaTime);
		isRunning = g_InputManager->ProcessInput();

		g_RenderManager->Update(deltaTime);
		g_InputManager->UpdatePrevInput();

		g_RenderManager->Render();

	}
	Shutdown();
}

// Separate from the destructor in case engine needs to restart(?)
void Engine::Shutdown() {
	g_DisplayManager->Shutdown();
	g_InputManager->Shutdown();
}