#pragma once
#include "EngineMath.h"
#include "Buffer.h"
#include <map>
#include <string>

class Texture {
public:
	// identifier for pre-calculations
	enum TexType {
		RGB, XYZ, BW
	};
	Texture(const char* filename, TexType type = TexType::RGB);
	~Texture();

	// loads image from filename
	void Load(const char* filename);

	// samples 3 floats in a row (RGB)
	Vector3 Sample(float u, float v);
	TexType type;
private:
	int width, height, channels;
	Buffer<float>* texBuffer;
};