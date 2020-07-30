#pragma once
#include <stdio.h>
#include <iostream>
// implementation of a templated buffer class for the z-buffer and the frame buffer
// NOTE: SDL Origin is top-left, Screens imagine it from bottom-left (.obj files)
// https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation
// NOTE: Pitch is the amount of memory in bytes of each row of the image essentially
// NOTE: Much simpler to overload ()(int, int) than [][] - per https://stackoverflow.com/questions/6969881/operator-overload
template <class T>
struct Buffer {
public:
	T* buffer;
	int pitch, numPixels, width, height;

	Buffer(int w, int h)
		: width(w)
		, height(h)
		, numPixels(w * h)
		, pitch(w * sizeof(T))
	{
		buffer = new T[width * height];
	}
	~Buffer() {
		delete buffer;
	}

	//origin is top left
	T& operator()(int x, int y) {
		int pos = y * width + x;
		return buffer[pos];
	}

	T& operator()(int pos) {
		return buffer[pos];
	}

	T* operator()(int x, int y, int len) {
		T* data = new T[len];
		int pos = y * width + x;
		for (int i = 0; i < len; i++) {
			data[i] = buffer[pos + i];
		}
		return data;
	}

	void clear() {
		if (std::is_same<T, float>::value) {
			for (int i = 0; i < numPixels; i++) {
				buffer[i] = 0.0f;
			}
		}
		else {
			memset(buffer, 0xD, pitch * height);

		}
	}
}; 