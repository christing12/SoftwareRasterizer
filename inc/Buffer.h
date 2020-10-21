#pragma once
#include <stdio.h>
#include <iostream>
// implementation of a templated buffer class for the Z-Buffer, Color Buffer, and Texture Buffers


/*
	Note: other types of buffers take in const void* as data and then a size
	but I need to set individual pixels 
	
*/
/*
	NOTE: Much simpler to overload ()(int, int) than [][] 
	- per https://stackoverflow.com/questions/6969881/operator-overload
*/
template <class T>
struct Buffer {
public:
	T* buffer;
	int width, height, stride;

	Buffer(int w, int h)
		: width(w)
		, height(h)
		, stride(w * sizeof(T))
	{
		buffer = new T[width * height];
	}

	~Buffer() {
		delete buffer;
	}

	

	// assumes origin is in the top left
	T& operator()(int x, int y) {
		int pos = y * width + x;
		return buffer[pos];
	}

	// accessor if index is already precalculated
	T& operator()(int pos) {
		return buffer[pos];
	}

	// probably unneeded, just the option of returning multiple queries of the buffer
	T* operator()(int x, int y, int len) {
		T* data = new T[len];
		int pos = y * width + x;
		for (int i = 0; i < len; i++) {
			data[i] = buffer[pos + i];
		}
		return data;
	}

	/*
		resets the buffer
		NOTE: there are some issues with memset and float
		- per https://stackoverflow.com/questions/37555614/memset-on-float-array-does-not-fully-zero-out-array
	*/
	void clear() {
		if (std::is_same<T, float>::value) {
			for (int i = 0; i < (width * height); i++) {
				buffer[i] = 0.0f;
			}
		}
		else {
			memset(buffer, 0xD, stride * height);

		}
	}
}; 