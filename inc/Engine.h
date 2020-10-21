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

	Ref<DisplayManager> g_DisplayManager;
	Ref<InputManager> g_InputManager;
	Ref<RenderManager> g_RenderManager;
};