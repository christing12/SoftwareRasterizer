#pragma once
class Scene;
class Renderer;
class DisplayManager;

class RenderManager {
public:
	RenderManager(Ref<DisplayManager> displayManager, Ref<class InputManager> inputManager);
	~RenderManager() {}
	bool Init(int w, int h, const char* = "../data/chest.json");
	void Shutdown();
	void Update(float deltaTime);

	// eventually to add scene switching
	Scene* GetCurrScene() { return m_currScene; }
	void SetCurrScene(Scene* s) { m_currScene = s; }

	void Render();

	bool wireframe			DEFAULT_INITIALIZER(false);
private:


	Scene* m_currScene		DEFAULT_INITIALIZER(nullptr);
	Ref<Renderer> _Renderer    DEFAULT_INITIALIZER(nullptr);


	Ref<DisplayManager> g_DisplayManager;
	Ref<class InputManager> g_InputManager;
};