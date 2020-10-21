#pragma once
class Texture;
class Shader;

// struct to hold texture information
struct Material {
	Material(std::string inType) : type(inType) {}
	
	~Material() = default;

	std::string type;

	Ref<Texture> Albedo					DEFAULT_INITIALIZER(nullptr);
	Ref<Texture> NormalMap				DEFAULT_INITIALIZER(nullptr);
	Ref<Texture> AmbientOcclusion		DEFAULT_INITIALIZER(nullptr);
	Ref<Texture> Metalness				DEFAULT_INITIALIZER(nullptr);
	Ref<Texture> Roughness				DEFAULT_INITIALIZER(nullptr);
};