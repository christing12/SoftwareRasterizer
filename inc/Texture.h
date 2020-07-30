#pragma once
#include "EngineMath.h"
#include "Buffer.h"
#include <map>
#include <string>

class Texture {
public:
	enum TexType {
		RGB, XYZ, BW
	};
	Texture(const char* filename, TexType type = TexType::RGB);
	~Texture();

	void Load(const char* filename);

	Vector3 Sample(float u, float v);
	float SampleF(float u, float v);
	TexType type;
private:
	int width, height, channels;
	Buffer<float>* texBuffer;
};