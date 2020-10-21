#pragma once
#include "EngineMath.h"
#include "Buffer.h"


enum class TEXTURE_TYPE : uint32_t {
	RGB = 0,
	XYZ,
	BW
};

class Texture {
public:
	// identifier for pre-calculations

	Texture(const char* filename, TEXTURE_TYPE = TEXTURE_TYPE::RGB);
	~Texture();

	// loads image from filename
	void Load(const char* filename);

	// samples 3 floats in a row (RGB)
	Vector3 Sample(float u, float v);
	TEXTURE_TYPE type;
private:
	int width, height, channels;
	Scope<Buffer<float> > _textureBuffer;
};