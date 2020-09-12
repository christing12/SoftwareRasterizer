#pragma once
#include "Geometry.h"
#include <vector>

class Mesh {
public:
	Mesh(const char* filename);
	~Mesh() {}
	size_t NumFaces();
	size_t NumVertices();

	const Vector3& GetFacetNormal(size_t idx);
	void GetFace(size_t idx, Vertex triangle[3], Vector3& facetNormal);

	const Vertex& GetVertex(size_t idx) { return m_vertices[idx]; }

private: 
	void LoadFile(const char* filename);
	std::vector<Vector3> m_faceNormals;
	std::vector<Vertex> m_vertices;


	size_t numFaces = 0;
	
	// pre-calculation helpers
	void CalculateFacetNormals();
	void CalculateTangentSpace();
};