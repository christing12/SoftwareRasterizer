#pragma once

#include <algorithm>
#include <iostream>
#include "Texture.h"
#include "Geometry.h"

void BaryInterpolate(Vector3& output, const Vector3 in[3], const Vector3& bary) {
	output.x = Dot(bary, Vector3(in[0].x, in[1].x, in[2].x));
	output.y = Dot(bary, Vector3(in[0].y, in[1].y, in[2].y));
	output.z = Dot(bary, Vector3(in[0].z, in[1].z, in[2].z));
}

struct Shader {
	virtual ~Shader() {};
	virtual Vector3 VertexShader(const Vector3& vert, const Vector3& texelCoords,
		const Vector3& norm, const Vector3& tangent, int index) = 0;
	virtual Vector3 FragmentShader(Vector3 bary, BBox box, Triangle& t) = 0;
};

// DOING PER VERTEX LIGHTING TO IMPROVE PERFORMANCE
struct PBRShader : public Shader {

	struct PBRUniforms {
		Matrix4 MVP, MV, V, M, N;
		Vector3 cameraPos;

		Ref<Texture> albedoT,  normalT,  aoTex,  metalT,  roughT;
		Vector3 lightPos, lightColor;
	};

	struct VOUT {
		Vector3 texCoord;
		Vector3 lightDir;
		Vector3 viewDir;
	};

	PBRUniforms uniforms;
	VOUT fragIn[3];

	//UNIFORM
	Matrix4 MVP, MV, V, M, N;
	Vector3 cameraPos;
	Texture* albedoT,  * normalMap, * ambientO, *  metal, * rough;
	Vector3 lightColor{ 1.f, 1.f, 1.f }, lightColorSpec{ 1.f, 1.f, 1.f };

	// VOUT
	Vector3 texels[3];
	Vector3 lightDir[3];
	Vector3 viewDir[3];

	// variables for vertex shader calculations
	Vector3 lightPos = Vector3::One;
	Vector3 worldPos;
	Vector3 normalWS, tangentWS, bitangentWS;
	Matrix4 TBN;



	// Trowbridge-Reitz GGX
	float NDF(float NdotH, float roughness) {
		float a2 = roughness * roughness;
		float NdotH2 = NdotH * NdotH;

		float denom = (NdotH2 * (a2 - 1.f) + 1.f);
		return a2 / denom;
	}

	float GeometrySchlickGGX(float NdotV, float k) {
		float denom = NdotV * (1.f - k) + k;
		return NdotV / denom;
	}

	float GeometrySmith(float NdotV, float NdotL, float roughness) {
		float a = roughness + 1.f;
		float k = (a * a) / 8.f;

		float ggx1 = GeometrySchlickGGX(NdotV, k);
		float ggx2 = GeometrySchlickGGX(NdotL, k);
		return ggx1 * ggx2;
	}

	Vector3 FresnelSchlick(float HdotV, Vector3 F0) {
		return (F0 + (Vector3::One - F0) * std::pow((1.f - HdotV), 5.f));
	}

	Vector3 VertexShader(const Vector3& vert,
		const Vector3& texelCoords,
		const Vector3& norm,
		const Vector3& tangent,
		int index
	)
	{
		worldPos = Transform(vert, M);
		texels[index] = texelCoords;

		normalWS = Normalize(Transform(norm, N));
		tangentWS = Normalize(Transform(tangent, N));
		bitangentWS = Cross(normalWS, tangentWS);
		TBN = Matrix4::TBNMatrix(tangentWS, bitangentWS, normalWS);

		/* If doing with OpenGL i would want the inverse of TBN to convert
			lighting variables into TBN space, but since with SR i can just
			do calculations in world space then transform into tangent space
			and do per-vertex lighting calculations to save on computation
		*/
		lightDir[index] = Transform(Normalize(lightPos), TBN);
		viewDir[index] = Transform(Normalize(cameraPos - worldPos), TBN);

		return Transform(vert, MVP);
	}

	// PER PIXEL
	Vector3 fragTexels, fragLight, fragView;
	Vector3 reflectDir;
	Vector3 rgb = Vector3(255.f, 255.f, 255.f);
	Vector3 diffuse, specular, ambient;

	Vector3 albedo, normal;
	float metalness, roughness, AO;

	unsigned int numLights = 1;
	Vector3 FDieletric = Vector3(0.04f, 0.04f, 0.04f);

	// All calculations are done in tangent space
	// Per-vertex lighting and then using interpolation
	Vector3 FragmentShader(Vector3 bary, BBox box, Triangle& t) {
		// Interpolation of fragment shader inputs
		BaryInterpolate(fragTexels, texels, bary);
		BaryInterpolate(fragLight, lightDir, bary);
		BaryInterpolate(fragView, viewDir, bary);
		fragLight.Normalize();
		fragView.Normalize();

		// All texture sampling
		albedo = albedoT->Sample(fragTexels.x, fragTexels.y);
		normal = normalMap->Sample(fragTexels.x, fragTexels.y);
		metalness = metal->Sample(fragTexels.x, fragTexels.y).x;
		roughness = rough->Sample(fragTexels.x, fragTexels.y).x;
		AO = ambientO->Sample(fragTexels.x, fragTexels.y).x;

		normal = normalMap->Sample(fragTexels.x, fragTexels.y);
		normal.Normalize();

		// other precaulcations
		Vector3 F0 = Lerp(FDieletric, albedo, metalness);
		float NdotV = std::max(0.f, Dot(normal, fragView));

		Vector3 directLighting = Vector3::Zero;


		for (unsigned int i = 0; i < numLights; i++) {
			Vector3 half = Normalize(fragLight + fragView);

			float NdotL = std::max(0.f, Dot(normal, fragLight));
			float NdotH = std::max(0.f, Dot(normal, half));
			float HdotV = std::max(0.f, Dot(half, fragView));

			Vector3 F = FresnelSchlick(HdotV, F0);
			float D = NDF(NdotH, roughness);
			float G = GeometrySmith(NdotV, NdotL, roughness);

			Vector3 kd = Lerp(Vector3::One, Vector3::Zero, metalness);
			Vector3 diffuseBRDF = kd * albedo;

			Vector3 specularBRDF = (F * D * G) / std::max(0.001f, 4.f * NdotL * NdotV);
			directLighting += (diffuseBRDF + specularBRDF) * lightColor * NdotL;
		}
		ambient = albedo * 0.01f * AO;

		return directLighting + ambient;
	}
};




/*
struct FlatShader : public Shader {
	Matrix4 MVP, MV, V, M, N;
	float varIntensity[3];
	Vector3 texels[3];
	Vector3 rgb = Vector3(255.f, 255.f, 255.f);
	Vector3 cameraPos;
	Texture* albedoT;

	Vector3 light = Vector3::One;

	Vector3 VertexShader(const Vector3& vert, const Vector3& texelCoords,
		const Vector3& norm, const Vector3& tangent, int index) override {
		Vector3 lightDir = Normalize(light);
		varIntensity[index] = std::max(0.f, Dot(Transform(norm, M), lightDir));
		texels[index] = texelCoords;
		return Transform(vert, MVP);
	}

	Vector3 textureColor, iUV;

	float iU, iV;
	Vector3 FragmentShader(Vector3 bary) {
		Vector3 vIntensity = Vector3(varIntensity[0], varIntensity[1], varIntensity[2]);

		iU = Dot(bary, Vector3(texels[0].x, texels[1].x, texels[2].x));
		iV = Dot(bary, Vector3(texels[0].y, texels[1].y, texels[2].y));

		textureColor = albedoT->Sample(iU, iV);


		float intensity = Dot(bary, vIntensity);
		Vector3 finColor = rgb * intensity * textureColor;
		return finColor;
	}
};
*/

// DOING PER VERTEX LIGHTING TO IMPROVE PERFORMANCE
/*
struct NormalShader : public Shader {

	//UNIFORM
	Matrix4 MVP, MV, V, M, N;
	Vector3 cameraPos;
	Texture* albedoT, *normalMap;
	Vector3 lightColor = Vector3::One;

	// VOUT
	Vector3 texels[3];
	Vector3 lightDir[3];
	Vector3 viewDir[3];
	float intensity[3];

	// variables for vertex shader calculations
	Vector3 lightPos = Vector3::One;
	Vector3 worldPos;
	Vector3 normalWS, tangentWS, bitangentWS;
	Matrix4 TBN;

	Vector3 VertexShader(const Vector3& vert,
		const Vector3& texelCoords,
		const Vector3& norm,
		const Vector3& tangent,
		int index
		)
	{
		worldPos = Transform(vert, M);
		texels[index] = texelCoords;

		normalWS = Normalize(Transform(norm, N, 0.f));
		tangentWS = Normalize(Transform(tangent, N, 0.f));
		bitangentWS = Cross(normalWS, tangentWS);
		TBN = Matrix4::TBNMatrix(tangentWS, bitangentWS, normalWS);

		/* If doing with OpenGL i would want the inverse of TBN to convert
			lighting variables into TBN space, but since with SR i can just
			do calculations in world space then transform into tangent space
			and do per-vertex lighting calculations to save on computation


		lightDir[index] = Transform(Normalize(lightPos), TBN, 0.f);
		viewDir[index] = Transform(Normalize(cameraPos - worldPos), TBN, 0.f);
		return Transform(vert, MVP);
	}

	// PER PIXEL
	Vector3 fragTexels, fragLight, fragView, fragNorm;
	Vector3 reflectDir;
	Vector3 rgb = Vector3(255.f, 255.f, 255.f);
	Vector3 diffuse, specular, ambient;
	Vector3 textureColor, normal;
	float ambientStrength = 0.4f, diffStrength = 0.9f, specStrength = 0.5f;
	float shininess = 32.f;
	float spec;

	Vector3 FragmentShader(Vector3 bary) {
		BaryInterpolate(fragTexels, texels, bary);
		BaryInterpolate(fragLight, lightDir, bary);
		BaryInterpolate(fragView, viewDir, bary);
		fragLight.Normalize();
		fragView.Normalize();

		textureColor = albedoT->Sample(fragTexels.x, fragTexels.y);

		normal = normalMap->Sample(fragTexels.x, fragTexels.y);
		normal.Normalize();

		//normal = Normalize((normal * 2.f) - Vector3::One);
		float NdotL = std::max(0.f, Dot(normal, fragLight));
		float NdotV = std::max(0.f, Dot(normal, fragView));


		// ambient
		ambient = ambientStrength * lightColor;

		// diffuse
		diffuse = lightColor * NdotL;
		Vector3 final = (diffuse)*textureColor;
		//return  final;

		Vector3 half = Normalize(fragLight + fragView);
		float NdotH = std::max(0.f, Dot(normal, half));
		spec = std::pow(NdotH, shininess);
		specular = lightColor * spec * specStrength;
		return (ambient + specular + diffuse) * textureColor;
	}
};
*/