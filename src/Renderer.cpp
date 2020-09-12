#include "Renderer.h"
#include "DisplayManager.h"
#include "RenderObj.h"
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
#include "Material.h"

#include <iostream>
#include <algorithm>
 #include <omp.h>




// static variables initalization
const SDL_PixelFormat* Renderer::format(SDL_AllocFormat(PIXEL_FORMAT));
const Uint32 Renderer::white(SDL_MapRGB(Renderer::format, 255, 255, 255));

//only needs to be calculated once->gamma values won't change for each pixel value
/* 
	https://learnopengl.com/Advanced-Lighting/Gamma-Correction
	https://www.scantips.com/lights/gamma3.html
*/
const int Renderer::gammaLUT[256] = { 0, 21, 28, 34, 39, 43, 46,
		50, 53, 56, 59, 61, 64, 66, 68, 70, 72, 74, 76, 78, 80, 82, 84,
		85, 87, 89, 90, 92, 93, 95, 96, 98, 99, 101, 102, 103, 105, 106,
		107, 109, 110, 111, 112, 114, 115, 116, 117, 118, 119, 120, 122,
		123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135,
		136, 137, 138, 139, 140, 141, 142, 143, 144, 144, 145, 146, 147,
		148, 149, 150, 151, 151, 152, 153, 154, 155, 156, 156, 157, 158,
		159, 160, 160, 161, 162, 163, 164, 164, 165, 166, 167, 167, 168,
		169, 170, 170, 171, 172, 173, 173, 174, 175, 175, 176, 177, 178,
		178, 179, 180, 180, 181, 182, 182, 183, 184, 184, 185, 186, 186,
		187, 188, 188, 189, 190, 190, 191, 192, 192, 193, 194, 194, 195,
		195, 196, 197, 197, 198, 199, 199, 200, 200, 201, 202, 202, 203,
		203, 204, 205, 205, 206, 206, 207, 207, 208, 209, 209, 210, 210,
		211, 212, 212, 213, 213, 214, 214, 215, 215, 216, 217, 217, 218,
		218, 219, 219, 220, 220, 221, 221, 222, 223, 223, 224, 224, 225,
		225, 226, 226, 227, 227, 228, 228, 229, 229, 230, 230, 231, 231,
		232, 232, 233, 233, 234, 234, 235, 235, 236, 236, 237, 237, 238,
		238, 239, 239, 240, 240, 241, 241, 242, 242, 243, 243, 244, 244,
		245, 245, 246, 246, 247, 247, 248, 248, 249, 249, 249, 250, 250,
		251, 251, 252, 252, 253, 253, 254, 254, 255, 255 };

// Inits the diff buffers
bool Renderer::Init(int width, int height) {
	w = width;
	h = height;
	g_displayManager = DisplayManager::Get();

	m_frameBuffer = new Buffer<Uint32>(width, height);
	m_zBuffer = new Buffer<float>(width, height);
	return true;
}

void Renderer::Shutdown() {
	delete m_frameBuffer;
	delete m_zBuffer;
}


void Renderer::ClearBuffers() {
	m_frameBuffer->clear();
	m_zBuffer->clear();
}

/*
	MAIN RENDERING FUNCTION
	performns Vertex shader (obj->clip space)
	performns (clip space->NDC->screen space)
	also backface culling
	sends it to rasterizer or wireframe renderer
*/
void Renderer::DrawObj(RenderObj* obj, Camera* cam, bool wireframe) {
	Mesh* m = obj->GetMesh();
	Material* mat = obj->GetMat();

	Matrix4 M = obj->transform;
	Matrix4 MV = cam->viewMatrix * M;
	Matrix4 MVP = cam->projectionMatrix * MV;

	Matrix4 worldToObj = obj->transform;
	worldToObj.Invert();

	// Setting up shader uniforms
	PBRShader shader;
	shader.MVP = MVP;
	shader.MV = MV;
	shader.V = cam->viewMatrix;
	shader.M = obj->transform;
	shader.N = Transpose(worldToObj);

	shader.albedoT = mat->albedoTex;
	shader.normalMap = mat->normalMap;
	shader.ambientO = mat->ambientO;
	shader.metal = mat->metal;
	shader.rough = mat->rough;

	shader.cameraPos = cam->cameraPos;


	/*
		I want the shader to 'reset' after each run of vertex->fragment
		private(shader) would make the shader inside the region random
		dynamic schedule for cases of backface culling
	*/
	#pragma omp parallel for firstprivate(shader) schedule(dynamic)
	for (int i = 0; i < m->NumFaces(); i++) {

		Vertex triangle[3];
		Vector3 facetNorm;

		m->GetFace(i, triangle, facetNorm);

		if (BackfaceCulling(triangle, facetNorm, worldToObj, cam)) continue;

		Triangle vsResult;
		Vertex v;
		for (int j = 0; j < 3; j++) {
			v = triangle[j];
			vsResult[j] = shader.VertexShader(v.pos, v.uv, v.normal, v.tangent, j);
		}

		// Clip Space -> Perspective Divide -> Screen Space
		Triangle screenSpaceVerts;
		float wVals[3];
		for (int j = 0; j < 3; j++) {
			wVals[j] = vsResult[j].w;
			vsResult[j] /= vsResult[j].w;
		}

		for (int j = 0; j < 3; j++) {
			screenSpaceVerts[j] = ViewportTransform(vsResult[j]);
		}

		//DRAWING PARTS
		if (wireframe) DrawTriangle(screenSpaceVerts, white);
		else DrawRasterTriangle(screenSpaceVerts, wVals, &shader);
	}
}

// Helper function to do wireframe rendering
void Renderer::DrawTriangle(Triangle& t, const Uint32& color) {

	DrawLine(t[0], t[1], color);
	DrawLine(t[1], t[2], color);
	DrawLine(t[2], t[0], color);
}

// Helper function for wireframe rendering using Bresenham's algorithm
void Renderer::DrawLine(const Vector3& p0, const Vector3& p1, const Uint32& color) {

	int x0 = int(p0.x);
	int y0 = int(p0.y);
	int x1 = int(p1.x);
	int y1 = int(p1.y);

	bool steep = false;
	if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	int dx = x1 - x0;
	int dy = y1 - y0;

	int derror2 = std::abs(dy) * 2;
	int error2 = 0;

	int y = y0;

	for (int x = x0; x <= x1; x++) {
		if (y < 0 || y >= (h - 1)) continue;
		if (x < 0 || x >= (w - 1)) continue;
		if (steep) {
			(*m_frameBuffer)(y, x) = color;
		}
		else {
			(*m_frameBuffer)(x, y) = color;
		}
		error2 += derror2;
		if (error2 > dx) {
			y += (y1 > y0 ? 1 : -1);
			error2 -= dx * 2;
		}
	}
}

// Only visualizes verts of the mesh
void Renderer::DrawModelVerts(Mesh* m, const Uint32& color) {
	/*for (int i = 0; i < m->NumVertices(); i++) {
		Vector3 v = m->GetVert(i);
		v = ViewportTransform(v);
		(*m_frameBuffer)(v.x, v.y) = color;
	} */

}

// Converts from NDC -> raster/screen space
Vector3 Renderer::ViewportTransform(const Vector3& v) {
	return Vector3((v.x + 1.f) * (w-1.f) * 0.5,
					(-v.y + 1.f) * (h-1.f) * 0.5,
					v.z);
}

// transforms NDC->raster for entire triangle
void Renderer::ViewportTransform(Triangle& t) {
	t[0] = ViewportTransform(t[0]);
	t[1] = ViewportTransform(t[1]);
	t[2] = ViewportTransform(t[2]);
}

// standard edge function to check what side of line a->b is point c on
// same as taking the cross of (b-a) & (c-a) and getting the z component
// mine returns negative if its on the inside
float Renderer::EdgeFunction(const Vector3& a, const Vector3& b, const Vector3& c) {
	Vector3 tB = b - a;
	Vector3 tC = c - a;
	return (tB.x * tC.y - tB.y * tC.x);
	//return Cross(b - a, c - a).z;
}

// Calcs bounding box of a triangle in screen space
BBox Renderer::GetBBox(Triangle& t) {
	int minX = std::min({ t[0].x, t[1].x, t[2].x });
	int minY = std::min({ t[0].y, t[1].y, t[2].y });

	int maxX = std::max({ t[0].x, t[1].x, t[2].x });
	int maxY = std::max({ t[0].y, t[1].y, t[2].y });

	// clamped against screen
	minX = std::max(minX, 0);
	minY = std::max(minY, 0);
	maxX = std::min(maxX, w - 1);
	maxY = std::min(maxY, h - 1);
	return BBox(Vector2(minX, minY), Vector2(maxX, maxY));
}

/*
	Main rasterization method(maybe put into own class?)
	https://stackoverflow.com/questions/24441631/how-exactly-does-opengl-do-perspectively-correct-linear-interpolation#:~:text=Perspective%20correct%20interpolation,-So%20let's%20say&text=This%20gives%20us%20the%20barycentric,by%20extension%2C%20world%20coordinates).
	performs rasterization and frag shader
*/
void Renderer::DrawRasterTriangle( Triangle& t, float wVals[3], Shader* shader) {
	// returns the area of the parallelogram formed by two of Triangle t
	// use as a denominator for bary coord
	float area = EdgeFunction(t[1], t[0], t[2]);
	area = 1.f / area;
	
	// like gl_FragCoord where the w = gl_Position.w;
	Vector3 recip_w = Vector3(1.f / wVals[0], 1.f / wVals[1], 1.f / wVals[2]);

	BBox bbox = GetBBox(t);
	Vector3 zCoord(t[0].z, t[1].z, t[2].z);

	// setting up linear incrementation of edge-rasterization algorithm
	float A01 = t[1].y - t[0].y, B01 = t[0].x - t[1].x;
	float A12 = t[2].y - t[1].y, B12 = t[1].x - t[2].x;
	float A20 = t[0].y - t[2].y, B20 = t[2].x - t[0].x;
	Vector3 edgeRow;
	Vector3 point(bbox.minX, bbox.minY, 0);
	edgeRow.x = EdgeFunction(t[2], t[1], point);
	edgeRow.y = EdgeFunction(t[0], t[2], point);
	edgeRow.z = EdgeFunction(t[1], t[0], point);

	Vector3 edge, weights;
	float depth, normalizer = 0;
	for (int y = bbox.minY; y <= bbox.maxY; y++) {
		edge.x = edgeRow.x;
		edge.y = edgeRow.y;
		edge.z = edgeRow.z;

		for (int x = bbox.minX; x <= bbox.maxX; x++) {
			if (edge.x >= 0.f && edge.y >= 0.f && edge.z >= 0.f) {

				//essentially percentages of the parallelogram(since both are x2 they cancel)
				Vector3 bary = edge * area;

				depth = Dot(bary, zCoord);

				if ((*m_zBuffer)(x, y) < depth && depth < 1.f) {
					(*m_zBuffer)(x, y) = depth;

					//perspective correct coordinates
					weights = bary * recip_w;
					normalizer = 1.f / (weights.x + weights.y + weights.z);
					bary = weights * normalizer;

					Vector3 rgb = shader->FragmentShader(bary, bbox, t);
					(*m_frameBuffer)(x, y) = GammaCorrect(rgb);
				}
			}
			edge.x += A12;
			edge.y += A20;
			edge.z += A01;
		}
		edgeRow.x += B12;
		edgeRow.y += B20;
		edgeRow.z += B01;
	}
}

// front-end culling using facet normals
bool Renderer::BackfaceCulling(Vertex t[3], const Vector3& faceNormal, const Matrix4& objMat, Camera* c) {
	Vector3 viewDir = Normalize(Transform(c->cameraPos, objMat) - t[0].pos);
	float intensity = Dot(faceNormal, viewDir);
	return intensity <= 0.f;
}

// Gamma Correct using gamma LUT (assumes rgb: [0, 1])
Uint32 Renderer::GammaCorrect(Vector3 rgb) {
	int gR = (int)(Math::Clamp(rgb.x * 255.f, 0.f, 255.f) + 0.5f);
	int gG = (int)(Math::Clamp(rgb.y * 255.f, 0.f, 255.f) + 0.5f);
	int gB = (int)(Math::Clamp(rgb.z * 255.f, 0.f, 255.f) + 0.5f);
	return SDL_MapRGB(format, gammaLUT[gR], gammaLUT[gG], gammaLUT[gB]);
}


// random color for testing
Uint32 Renderer::RandColor() {
	return SDL_MapRGB(g_displayManager->GetPixelFormat(), rand() % 255, rand() % 255, rand() % 255);
}