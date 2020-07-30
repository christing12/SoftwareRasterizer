#pragma once
#include "nlohmann/json_fwd.hpp"
#include <vector>
#include <string>
#include "EngineMath.h"
class Camera;
class RenderObj;
class Mesh;
class Material;

struct Light {
	Vector3 position;
	Vector3 color;
};


class Scene {
public:

	Scene(const char* filename);
	~Scene();

	void LoadScene(const char* filename);
	void Update(float deltaTime);

	void AddObj(RenderObj* obj);
	void CreateObj(Mesh* m);
	Camera* GetCamera() { return m_mainCam; }
	std::vector<RenderObj*>& GetRenderObjs() { return m_renderObjs; }
private:
	Camera* m_mainCam;
	std::vector<RenderObj*> m_renderObjs;
	std::vector<Light> m_lights;

	void GetTextureFromJSON(Material* mat, std::string key, nlohmann::json jsonObj, std::string type);
	Material* GetMaterialFromJSON(nlohmann::json matJSON);
};