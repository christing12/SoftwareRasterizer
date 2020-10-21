#include "stdafx.h"

#include "RenderManager.h"
#include "Renderer.h"
#include "RenderObj.h"
#include "Scene.h"
#include "DisplayManager.h"

RenderManager::RenderManager(Ref<DisplayManager> displayManager, Ref<class InputManager> inputManager)
	: g_DisplayManager(displayManager)
	, g_InputManager(inputManager)
{

}

bool RenderManager::Init(int w, int h, const char* initialScene) {
	_Renderer = CreateRef<Renderer>(g_DisplayManager);
	if (!_Renderer->Init(w, h)) {
		std::cout << "SOMETHING WENT WRONG WITH RENDER MAANGER" << std::endl;
		return false;
	}
	std::cout << "Render Manager being initted" << std::endl;
	m_currScene = new Scene(initialScene, g_InputManager);
	return true;
}

void RenderManager::Shutdown() {
	_Renderer->Shutdown();
	delete m_currScene;
}

// regular delta time update for the current scene & and its objects
void RenderManager::Update(float deltaTime) {
	m_currScene->Update(deltaTime);
}

// clears the zbuffer/back buffer, init rendering process, and update the display
void RenderManager::Render() {
	_Renderer->ClearBuffers();
	std::vector<RenderObj*> objs = m_currScene->GetVisibleObjs();

	for (RenderObj* obj : objs) {
		_Renderer->DrawObj(obj, m_currScene->GetCamera(), wireframe);
	}

	g_DisplayManager->UpdateDisplay(_Renderer->GetFrameBuffer());
}