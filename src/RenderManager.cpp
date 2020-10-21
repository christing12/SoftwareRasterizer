#include "stdafx.h"
#include <vector>
#include <iostream>
#include "RenderManager.h"
#include "Renderer.h"
#include "RenderObj.h"
#include "Scene.h"
#include "DisplayManager.h"

bool RenderManager::Init(int w, int h) {
	m_renderer = Renderer::Get();
	if (!m_renderer->Init(w, h)) {
		std::cout << "SOMETHING WENT WRONG WITH RENDER MAANGER" << std::endl;
		return false;
	}
	std::cout << "Render Manager being initted" << std::endl;
	g_displayManager = DisplayManager::Get();
	m_currScene = new Scene("../data/chest.json");
	return true;
}

void RenderManager::Shutdown() {
	m_renderer->Shutdown();
	delete m_currScene;
}

// regular delta time update for the current scene & and its objects
void RenderManager::Update(float deltaTime) {
	m_currScene->Update(deltaTime);
}

// clears the zbuffer/back buffer, init rendering process, and update the display
void RenderManager::Render() {
	m_renderer->ClearBuffers();
	std::vector<RenderObj*> objs = m_currScene->GetVisibleObjs();

	for (RenderObj* obj : objs) {
		m_renderer->DrawObj(obj, m_currScene->GetCamera(), wireframe);
	}
	g_displayManager->UpdateDisplay(m_renderer->GetFrameBuffer());
}