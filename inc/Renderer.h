#pragma once

#include <SDL.h>
#include "Geometry.h"
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

	// class constant info
	static const Uint32 white;
	static const SDL_PixelFormat* format;
	static const Uint32 PIXEL_FORMAT = SDL_PIXELFORMAT_RGB888;
	static const int gammaLUT[256];


	// Main Draw Function (Vertex -> Rasterization -> Fragment)
	void DrawObj(RenderObj* obj, Camera* cam, bool wireframe = false);


	// BUFFER STUFF 
	Buffer<Uint32>* GetFrameBuffer() { return m_frameBuffer;  }
	Buffer<float>* GetZBuffer() { return m_zBuffer; }
	void ClearBuffers();

private: 
	Renderer() {}
	~Renderer() {}

	int w, h;

	class DisplayManager* g_displayManager;
	Buffer<Uint32>* m_frameBuffer;
	Buffer<float>* m_zBuffer;



	BBox GetBBox(Triangle& t);
	
	// NDC -> Screen Space
	Vector3 ViewportTransform(const Vector3& v);
	void ViewportTransform(Triangle& t);
	
	// helper functions with simple drawing
	void DrawRasterTriangle(Triangle& t, float wVals[3], Shader* shader);
	void DrawLine(const Vector3& p0, const Vector3& p1, const Uint32& color);
	void DrawTriangle(Triangle& t, const Uint32& color);
	void DrawModelVerts(Mesh* m, const Uint32& color);

	bool BackfaceCulling(Vertex t[3], const Vector3& faceNormal, const Matrix4& objMat, Camera* c);

	// gamma corrects color basedon gamma LUT
	Uint32 GammaCorrect(Vector3 rgb);
	float EdgeFunction(const Vector3& a, const Vector3& b, const Vector3& c);


	Uint32 RandColor();
};