#pragma once
class Texture;
class Shader;

// struct to hold texture information
struct Material {
	Material(std::string inType) : type(inType) {}
	
	~Material() {
		if (albedoTex)	delete albedoTex;
		if (normalMap)	delete normalMap;
		if (ambientO)	delete ambientO;
		if (metal)		delete metal;
		if (rough)		delete rough;
	}

	std::string type;

	Texture* albedoTex;
	Texture* normalMap;
	Texture* ambientO;
	Texture* metal;
	Texture* rough;
};