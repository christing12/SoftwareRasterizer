#pragma once

#include <SDL.h>
#include "EngineMath.h"
#include "Buffer.h"

class Mesh;
class RenderObj;
class Camera;
class Shader;

class Renderer {
public:
	static Renderer* Get() {
		static Renderer manager;
		return &manager;
	}

	bool Init(int width, int height);
	void Shutdown();

	// CONSTANT COLORS
	static const Uint32 white;
	static const SDL_PixelFormat* format;
	static const Uint32 PIXEL_FORMAT = SDL_PIXELFORMAT_RGB888;
	static const int gammaLUT[256];


	// TRANSFORM + RASTERIZAION FUNC
	void DrawObj(RenderObj* obj, Camera* cam, bool wireframe = false);
	void DrawModelVerts(Mesh* m, const Uint32& color);


	// BUFFER STUFF 
	Buffer<Uint32>* GetFrameBuffer() { return m_frameBuffer;  }
	Buffer<float>* GetZBuffer() { return m_zBuffer; }
	void ClearBuffers();

	float EdgeFunction(const Vector3& a, const Vector3& b, const Vector3& c);
	bool Inside(Triangle& t, const Vector3& p);
private: 
	Renderer() {}
	~Renderer() {}

	class DisplayManager* g_displayManager;
	Buffer<Uint32>* m_frameBuffer;
	Buffer<float>* m_zBuffer;

	int w, h;

	BBox GetBBox(Triangle& t);
	Vector3 ViewportTransform(const Vector3& v);
	void ViewportTransform(Triangle& t);

	
	// helper functions
	void DrawRasterTriangle(Triangle& t, float wVals[3], Shader* shader);
	void DrawLine(const Vector3& p0, const Vector3& p1, const Uint32& color);
	void DrawTriangle(Triangle& t, const Uint32& color);
	bool BackfaceCulling(Vertex t[3], const Vector3& faceNormal, const Matrix4& objMat, Camera* c);

	Uint32 RandColor();
	Uint32 GammaCorrect(Vector3 rgb);
};