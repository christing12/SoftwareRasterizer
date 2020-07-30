#pragma once

class DisplayManager;
class InputManager;
class Renderer;
class RenderManager;


class Engine {
public:
	Engine() {}
	~Engine() {}

	bool Init();
	void Run();
	void Shutdown();
private:
	bool isRunning = true;
	DisplayManager* g_displayManager;
	InputManager* g_inputManager;
	RenderManager* g_renderManager;


};