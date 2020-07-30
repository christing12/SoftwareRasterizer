#include "Mesh.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>


Mesh::Mesh(const char* filename) {
	LoadFile(filename);
}

void Mesh::LoadFile(const char* filename) {
	std::ifstream in;
	in.open(filename, std::ifstream::in);
	if (in.fail()) {
		std::cout << "ISSUES WITH FILE" << std::endl;
		return;
	}

	std::vector<Vector3> tempVerts;
	std::vector<Vector3> tempUVs;
	std::vector<Vector3> tempNormals;
	std::vector<unsigned int> vIndices, uvIndices, normIndices;


	std::string line, key;
	std::string x, y, z;

	while (!in.eof()) {
		std::getline(in, line);
		std::istringstream iss(line);
		char trash;
		iss >> key;
		if (key == "v") {
			Vector3 v;
			iss >> v.x >> v.y >> v.z;
			tempVerts.push_back(v);
		}
		else if (key == "vt") {
			Vector3 v;
			iss >> v.x >> v.y >> v.z;
			tempUVs.push_back(v);
		}
		else if (key == "vn") {
			Vector3 norm;
			iss >> norm.x >> norm.y >> norm.z;
			tempNormals.push_back(norm);
		}

		//NOTE THE X = VERT INDEX, Y = TEXCOORD IDX, Z NORM IDX
		else if (key == "f") {
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			for (int i = 0; i < 3; i++) {
				iss >> vertexIndex[i] >> trash >>  uvIndex[i] >> trash >> normalIndex[i];
			}

			vIndices.push_back(vertexIndex[0]);
			vIndices.push_back(vertexIndex[1]);
			vIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normIndices.push_back(normalIndex[0]);
			normIndices.push_back(normalIndex[1]);
			normIndices.push_back(normalIndex[2]);

			numFaces++;
		}
	}


	for (unsigned int i = 0; i < vIndices.size(); i++) {
		Vertex v;
		v.pos = tempVerts[vIndices[i] - 1];
		v.uv = tempUVs[uvIndices[i] - 1];
		v.normal = tempNormals[normIndices[i] - 1];
		m_vertices.push_back(v);
	}
	CalculateFacetNormals();
	CalculateTangentSpace();
}


// USED FOR BACKFACE CULLING SO I CAN AVOID CALCULATING IT EVERY FRAME
void Mesh::CalculateFacetNormals() {
	int idx;
	for (size_t i = 0; i < numFaces; i++) {
		idx = i * 3;
		Vector3 a = m_vertices[idx+1].pos - m_vertices[idx].pos;
		Vector3 b = m_vertices[idx+2].pos - m_vertices[idx].pos;
		Vector3 cross = Cross(a, b);
		cross.Normalize();
		m_faceNormals.push_back(cross);
	}
}

void Mesh::CalculateTangentSpace() {
	unsigned int numVerts = NumVertices();
	std::vector<Vector3> tangent(numVerts, Vector3::Zero);
	std::vector<Vector3> bitangent(numVerts, Vector3::Zero);

	for (int i = 0; i < numVerts; i++) {
		tangent[i] = Vector3::Zero;
		bitangent[i] = Vector3::Zero;
	}
	unsigned int i0;
	for (int i = 0; i < numFaces; i++) {
		i0 = i * 3;


		Vertex face[3];
		Vector3 facetNormal;
		GetFace(i, face, facetNormal);

		Vector3 edge1 = face[1].pos - face[0].pos;
		Vector3 edge2 = face[2].pos - face[0].pos;

		float x1 = face[1].uv.x - face[0].uv.x, x2 = face[2].uv.x - face[0].uv.x;
		float y1 = face[1].uv.y - face[0].uv.y, y2 = face[2].uv.y - face[0].uv.y;

		float r = 1.f / (x1 * y2 - x2 * y1);
		Vector3 t = (edge1 * y2 - edge2 * y1) * r;
		Vector3 b = (edge2 * x1 - edge1 * x2) * r;

		tangent[i0] += t;
		tangent[i0 + 1] += t;
		tangent[i0 + 2] += t;
		
		bitangent[i0] += b;
		bitangent[i0 + 1] += b;
		bitangent[i0 + 2] += b; 
	}

	for (unsigned int i = 0; i < numVerts; i++) {
		Vertex v = m_vertices[i];
		const Vector3& t = tangent[i];
		const Vector3& b = bitangent[i];
		const Vector3& n = v.normal;

		Vector3 newTangent = Normalize(t - n * Dot(n, t));
		newTangent.w = (Dot(Cross(n, t), b) < 0.f) ? -1.f : 1.f;
		m_vertices[i].tangent = newTangent;
	}
}

size_t Mesh::NumFaces() {
	return numFaces;
}

size_t Mesh::NumVertices() {
	return m_vertices.size();
}

const Vector3& Mesh::GetFacetNormal(size_t idx) {
	return m_faceNormals[idx];
}


void Mesh::GetFace(size_t idx, Vertex triangle[3], Vector3& facetNormal) {
	size_t index = idx * 3;
	for (unsigned int i = 0; i < 3; i++) {
		triangle[i] = m_vertices[index + i];
	}
	facetNormal = m_faceNormals[idx];
}
