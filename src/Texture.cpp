#include "stdafx.h"
#include "Texture.h"
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

// stb_image load
void Texture::Load(const char* filename) {
	stbi_set_flip_vertically_on_load(true);
    
    // converting the unsgined chars to float will be from 0-255
	unsigned char* img = stbi_load(filename, &width, &height, &channels, 0);
    std::cout << channels << std::endl;
	if (img == nullptr) {
		std::cout << " SOMETHING WRONG WITH LOADINTG HTE IMAGE" << std::endl;
		return;
	}

    texBuffer = new Buffer<float>(width * channels, height);

    /* Do pre calculations for the 3 typees of textures I currently take
        RGB: convert from [0, 255] to [0, 1] then pre-calculate gamma correction
        Normal Map (XYZ): usually done on the fragment/pixel shader but pre-calculates
        BW: for things like metalness / AO / roughness where we just need intensity value
            so just map from [0, 255] -> [0, 1]
     */

    // pre-gamma correction -> saves calculations later
	if (type == TexType::RGB) {
		for (int i = 0; i < width * height * channels; ++i) {
            texBuffer->buffer[i] = std::pow(float(img[i]) * (1 / 255.f), 2.2f);
		}
	} 
    // Pre-maps normals from [0, 1] -> [-1, 1] (saves computations inside
	else if (type == TexType::XYZ) {
		for (int i = 0; i < width * height * channels; ++i) {
            texBuffer->buffer[i] = float(img[i]) * (2.f / 255.f) - 1.f;
		}
	}
    // simple conversion to 
    else if (type == TexType::BW) {
        for (int i = 0; i < width * height * channels; ++i) {
            texBuffer->buffer[i] = float(img[i]) * (1.f / 255.f);
        }
    }
	stbi_image_free(img);
}


// fujll RGB color sample for when there are 3 channels
Vector3 Texture::Sample(float u, float v) {
    //if (u < 0.f || u > 1.f) {
    //    std::cout << " SOMETHING WENT WRONG " << std::endl;
    //}
    int x = int(u * (width - 1.f)) * channels;
    int y = v * (height - 1);

    // same as doing pos = y * width + x;
    int i = (y * width * channels) + x;
    return Vector3((*texBuffer)(i), (*texBuffer)(i + 1), (*texBuffer)(i + 2));

}
