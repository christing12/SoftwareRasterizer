#pragma once
#include "nlohmann/json_fwd.hpp"
#include <vector>
#include <string>
#include "EngineMath.h"
class Camera;
class RenderObj;
class Mesh;
class Material;
class InputManager;

struct Light {
	Vector3 position;
	Vector3 color;
};


class Scene {
public:

	Scene(const char* filename, Ref<InputManager> inputManager);
	~Scene();

	void LoadScene(const char* filename);
	void Update(float deltaTime);

	Camera* GetCamera() { return m_mainCam; }
	std::vector<Ref<RenderObj> >& GetRenderObjs() { return _renderObjs; }

	void FrustumCulling();

	std::vector<RenderObj*> GetVisibleObjs();
private:
	Ref<InputManager> g_InputManager;

	Camera* m_mainCam;
	std::vector<Ref<RenderObj> > _renderObjs;
	std::vector<Light> m_lights;

	void GetTextureFromJSON(Material* mat, std::string key, nlohmann::json jsonObj, std::string type);
	Ref<Material> GetMaterialFromJSON(nlohmann::json matJSON);
};