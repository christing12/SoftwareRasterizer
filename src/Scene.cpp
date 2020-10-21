#include "stdafx.h"

#include "Scene.h"
#include "RenderObj.h"
#include "Mesh.h"
#include "Camera.h"
#include "Texture.h"
#include "Material.h"
#include "nlohmann/json.hpp"
#include "InputManager.h"


using json = nlohmann::json;

Scene::Scene(const char* filename, Ref<InputManager> inputManager)
	: g_InputManager(inputManager)
{
	LoadScene(filename);
}

Scene::~Scene() {
	delete m_mainCam;
}

void Scene::LoadScene(const char* filename) {
	std::cout << filename << std::endl;
	std::ifstream ifs(filename);
	if (ifs.fail()) {
		std::cout << " THERE ARE SOME ISSUES" << std::endl;
	}
	json scene = json::parse(ifs);

	json metadata = scene["metadata"];
	std::string type = metadata["type"];
	std::string filepath = metadata["filepath"];
	

	// camera
	{
		json cameraData = scene["camera"];
		json position = cameraData["position"];
		Vector3 translation = Vector3(float(position[0]), float(position[1]), float(position[2]));
		m_mainCam = new Camera(translation, g_InputManager);
	}

	// parsing all object information
	{
		size_t numObjs = scene["renderObjects"].size();
		for (size_t i = 0; i < numObjs; i++) {
			json obj = scene["renderObjects"][i];
			
			json position = obj["position"];
			Vector3 translation = Vector3(float(position[0]), float(position[1]), float(position[2]));
			Matrix4 transMat = Matrix4::CreateTranslation(translation);

			json rotation = obj["rotation"];
			Vector3 rot = Vector3(Math::ToRadians(float(rotation[0])),
				Math::ToRadians(float(rotation[1])), Math::ToRadians(float(rotation[2])));
			Matrix4 rotMat = Matrix4::CreateYawPitchRoll(rot.x, rot.y, rot.z);

			json scale = obj["scale"];
			Vector3 vScale = Vector3(float(scale[0]), float(scale[1]), float(scale[2]));
			Matrix4 scaleMat = Matrix4::CreateScale(vScale);

			

			std::string meshFile = obj["mesh"];
			Ref<Mesh> mesh = CreateRef<Mesh>(meshFile.c_str());


			Ref<Material> mat = GetMaterialFromJSON(obj["material"]);
			Ref<RenderObj> rObj = CreateRef<RenderObj>(mat, mesh);
			rObj->transform = transMat;
			_renderObjs.push_back(rObj);
		}

		// parsing the lights in the scene
		{
			size_t numLights = scene["lights"].size();
			for (size_t i = 0; i < numLights; i++) {
				json lightJSON = scene["lights"][i];

				json position = lightJSON["position"];
				Vector3 translation = Vector3(float(position[0]), float(position[1]), float(position[2]));

				json colorJSON = lightJSON["color"];
				Vector3 color = Vector3(float(colorJSON[0]), float(colorJSON[1]), float(colorJSON[2]));

				Light l{ translation, color };
				m_lights.push_back(l);
			}
		}
	}
}

void Scene::Update(float deltaTime) {
	if (g_InputManager->MouseButtonPressed(MOUSE_BUTTON::LEFT)) {
		Vector2 mousePos = g_InputManager->MousePos();
		std::cout << "Mouse Pos: " << mousePos.x << " " << mousePos.y << std::endl;
	}
	m_mainCam->Update(deltaTime);
	for (Ref<RenderObj> obj : _renderObjs) {
		obj->Update(deltaTime);
	}
	FrustumCulling();
}

void Scene::FrustumCulling() {
	for (Ref<RenderObj> obj : _renderObjs) {

	}
}


// Helper function currently not being used
void Scene::GetTextureFromJSON(Material* mat, std::string key, nlohmann::json jsonObj, std::string type) {
	auto iter = jsonObj.find(key);
	if (iter != jsonObj.end()) {
		std::string filepath = jsonObj[key];
		//Texture* tex = new Texture(filepath.c_str(), Texture::texType[type]);
	}
}

Ref<Material> Scene::GetMaterialFromJSON(json matJSON) {
	std::string type = matJSON["type"];
	Ref<Material> mat = CreateRef<Material>(type);


	std::string albedoFile = matJSON["albedo"];
	mat->Albedo = CreateRef<Texture>(albedoFile.c_str(), TEXTURE_TYPE::RGB);

	auto iter = matJSON.find("normal");
	if (iter != matJSON.end()) {
		std::string normalFile = matJSON["normal"];
		mat->NormalMap = CreateRef<Texture>(normalFile.c_str(), TEXTURE_TYPE::XYZ);
	}

	if (matJSON.find("metal") != matJSON.end() &&
		matJSON.find("rough") != matJSON.end() &&
		matJSON.find("AO") != matJSON.end()) {
		std::string metalPath = matJSON["metal"];
		std::string roughPath = matJSON["rough"];
		std::string aoPath = matJSON["AO"];

		Ref<Texture> metal	= CreateRef<Texture>(metalPath.c_str(), TEXTURE_TYPE::BW);
		Ref<Texture> rough	= CreateRef<Texture>(roughPath.c_str(), TEXTURE_TYPE::BW);
		Ref<Texture> ao		= CreateRef<Texture>(aoPath.c_str(), TEXTURE_TYPE::BW);

		mat->Metalness = metal;
		mat->Roughness = rough;
		mat->AmbientOcclusion = ao;
	}
	return mat;
}

std::vector<RenderObj*> Scene::GetVisibleObjs() {
	std::vector<RenderObj*> visObjs;
	for (Ref<RenderObj> obj : _renderObjs) {
		if (m_mainCam->isVisible(obj.get())) visObjs.push_back(obj.get());
	}
	return visObjs;
}
