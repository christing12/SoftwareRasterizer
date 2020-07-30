#include "Scene.h"
#include "RenderObj.h"
#include "Mesh.h"
#include "Camera.h"
#include "Texture.h"
#include "Material.h"
#include "nlohmann/json.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <map>

using json = nlohmann::json;

Scene::Scene(const char* filename) {
	LoadScene(filename);
}

Scene::~Scene() {
	for (RenderObj* obj : m_renderObjs) {
		delete obj;
	}
	delete m_mainCam;
}

void Scene::LoadScene(const char* filename) {
	std::ifstream ifs(filename);
	json scene = json::parse(ifs);

	json metadata = scene["metadata"];
	std::string type = metadata["type"];
	std::string filepath = metadata["filepath"];
	

	// camera
	{
		json cameraData = scene["camera"];
		json position = cameraData["position"];
		Vector3 translation = Vector3(float(position[0]), float(position[1]), float(position[2]));
		m_mainCam = new Camera(translation);
	}

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
			Mesh* mesh = new Mesh(meshFile.c_str());

			Material* mat = GetMaterialFromJSON(obj["material"]);
			RenderObj* rObj = new RenderObj(mat, mesh);
			rObj->transform = transMat;
			AddObj(rObj);
		}

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
	for (RenderObj* obj : m_renderObjs) {
		obj->Update(deltaTime);
	}
}

void Scene::AddObj(RenderObj* obj) {
	m_renderObjs.push_back(obj);
}

void Scene::CreateObj(Mesh* m) {
	//AddObj(new RenderObj(m));
}

void Scene::GetTextureFromJSON(Material* mat, std::string key, nlohmann::json jsonObj, std::string type) {
	auto iter = jsonObj.find(key);
	if (iter != jsonObj.end()) {
		std::string filepath = jsonObj[key];
		//Texture* tex = new Texture(filepath.c_str(), Texture::texType[type]);
	}
}

Material* Scene::GetMaterialFromJSON(json matJSON) {
	std::string type = matJSON["type"];
	Material* mat = new Material(type);

	std::string albedoFile = matJSON["albedo"];
	Texture* albedo = new Texture(albedoFile.c_str(), Texture::TexType::RGB);
	mat->albedoTex = albedo;

	auto iter = matJSON.find("normal");
	if (iter != matJSON.end()) {
		std::string normalFile = matJSON["normal"];
		Texture* normalMap = new Texture(normalFile.c_str(), Texture::TexType::XYZ);
		mat->normalMap = normalMap;
	}

	if (matJSON.find("metal") != matJSON.end() &&
		matJSON.find("rough") != matJSON.end() &&
		matJSON.find("AO") != matJSON.end()) {
		std::string metalPath = matJSON["metal"];
		std::string roughPath = matJSON["rough"];
		std::string aoPath = matJSON["AO"];

		Texture* metal = new Texture(metalPath.c_str(), Texture::TexType::BW);
		Texture* rough = new Texture(roughPath.c_str(), Texture::TexType::BW);
		Texture* ao = new Texture(aoPath.c_str(), Texture::TexType::BW);

		mat->metal = metal;
		mat->rough = rough;
		mat->ambientO = ao;
	}

	return mat;

}
