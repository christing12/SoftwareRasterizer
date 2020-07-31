#pragma once
class Scene;
class Renderer;
class DisplayManager;

// Singleton class design, decided the renderer itself shouldn't manage all this and only focus on rendering
class RenderManager {
public:
	static RenderManager* Get() {
		static RenderManager manager;
		return &manager;
	}

	bool Init(int w, int h);
	void Shutdown();
	void Update(float deltaTime);

	// eventually to add scene switching
	Scene* GetCurrScene() { return m_currScene; }
	void SetCurrScene(Scene* s) { m_currScene = s; }

	void Render();

	bool wireframe = false;
private:
	RenderManager() {}
	~RenderManager() {}

	Scene* m_currScene;
	Renderer* m_renderer;
	DisplayManager* g_displayManager;
};