#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <cmath>
#include <iostream>

 std::map<std::string, Texture::TexType> texType =
{
    { "RGB", Texture::TexType::RGB },
    { "XYZ", Texture::TexType::XYZ },
    { "BW", Texture::TexType::BW }
};

Texture::Texture(const char* filename, Texture::TexType inType) : type(inType){
	Load(filename);
}

Texture::~Texture() {
    if (texBuffer) delete texBuffer;
}

void Texture::Load(const char* filename) {
	stbi_set_flip_vertically_on_load(true);
	unsigned char* img = stbi_load(filename, &width, &height, &channels, 0);
	if (img == nullptr) {
		std::cout << " SOMETHING WRONG WITH LOADINTG HTE IMAGE" << std::endl;
		return;
	}

    texBuffer = new Buffer<float>(width * channels, height);

    // gamma corrects RGB colors to [0, 1]
	if (type == TexType::RGB) {
		for (int i = 0; i < width * height * channels; ++i) {
			float data = std::pow((float)img[i] * (1 / 255.f), 2.2f);
            texBuffer->buffer[i] = data;
		}
	} 
    // Pre-maps normals from [0, 1] -> [-1, 1]
	else if (type == TexType::XYZ) {
		for (int i = 0; i < width * height * channels; ++i) {
			float data = (float)img[i] * (2.f / 255.f) - 1.f;
            texBuffer->buffer[i] = data;

		}
	}
    else if (type == TexType::BW) {
        for (int i = 0; i < width * height * channels; ++i) {
            float data = (float)img[i] * (1.f / 255.f);
            texBuffer->buffer[i] = data;
        }
    }
	stbi_image_free(img);
}



Vector3 Texture::Sample(float u, float v) {
    int x = int(u * (width - 1.f)) * channels;
    int y = v * (height - 1);

    // same as doing pos = y * width + x;
    int i = (y * width * channels) + x;
    return Vector3((*texBuffer)(i), (*texBuffer)(i + 1), (*texBuffer)(i + 2));

}
float Texture::SampleF(float u, float v) {
    int x = int(u * (width - 1)) * channels;
    int y = v * (height - 1);
    return (*texBuffer)(x, y);
}