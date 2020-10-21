#include "stdafx.h"
#include "Texture.h"
#include "stb_image.h"
#include <cmath>
#include <iostream>


Texture::Texture(const char* filename, TEXTURE_TYPE inType) : type(inType){
	Load(filename);
}

Texture::~Texture() {
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

    _textureBuffer = CreateScope<Buffer<float> >(width * channels, height);

    /* Do pre calculations for the 3 typees of textures I currently take
        RGB: convert from [0, 255] to [0, 1] then pre-calculate gamma correction
        Normal Map (XYZ): usually done on the fragment/pixel shader but pre-calculates
        BW: for things like metalness / AO / roughness where we just need intensity value
            so just map from [0, 255] -> [0, 1]
     */
    switch (type) {
        case TEXTURE_TYPE::RGB: 
        {
            for (int i = 0; i < width * height * channels; ++i) {
                _textureBuffer->buffer[i] = std::pow(float(img[i]) * (1 / 255.f), 2.2f);
            }
            break;
        }
        case TEXTURE_TYPE::XYZ:
        {
            for (int i = 0; i < width * height * channels; ++i) {
                _textureBuffer->buffer[i] = float(img[i]) * (2.f / 255.f) - 1.f;
            }
            break;
        }
        case TEXTURE_TYPE::BW:
        {
            for (int i = 0; i < width * height * channels; ++i) {
                _textureBuffer->buffer[i] = float(img[i]) * (1.f / 255.f);
            }
            break;
        }
    }
	stbi_image_free(img);
}


// fujll RGB color sample for when there are 3 channels
Vector3 Texture::Sample(float u, float v) {
    int x = int(u * (width - 1.f)) * channels;
    int y = v * (height - 1);

    // same as doing pos = y * width + x;
    int i = (y * width * channels) + x;
    return Vector3((*_textureBuffer)(i), (*_textureBuffer)(i + 1), (*_textureBuffer)(i + 2));

}
