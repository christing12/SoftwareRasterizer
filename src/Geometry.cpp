#include "stdafx.h"
#include "Geometry.h"
#include "Mesh.h"

#include <algorithm>

AABB::AABB(Mesh* mesh) 
	: min(std::numeric_limits<float>::max())
	, max(std::numeric_limits<float>::min())
{
	for (unsigned int i = 0; i < mesh->NumVertices(); i++) {
		Vertex v = mesh->GetVertex(i);
		max.x = std::max(v.pos.x, max.x);
		max.y = std::max(v.pos.y, max.y);
		max.z = std::max(v.pos.z, max.z);

		min.x = std::min(v.pos.x, min.x);
		min.y = std::min(v.pos.y, min.y);
		min.z = std::min(v.pos.z, min.z);
	}
}


void AABB::ConstructVerts(Vector3 verts[]) {
	verts[0] = Vector3(min.x, min.y, min.z);
	verts[1] = Vector3(max.x, min.y, min.z);
	verts[2] = Vector3(min.x, max.y, min.z);
	verts[3] = Vector3(max.x, max.y, min.z);
	
	verts[4] = Vector3(min.x, min.y, max.z);
	verts[5] = Vector3(max.x, min.y, max.z);
	verts[6] = Vector3(min.x, max.y, max.z);
	verts[7] = Vector3(max.x, max.y, max.z);
}

void AABB::Update() {
	Vector3 verts[8];
	ConstructVerts(verts);

	min = Vector3(std::numeric_limits<float>::max());
	max = Vector3(std::numeric_limits<float>::min());

	for (unsigned int i = 0; i < 8; i++) {
		max.x = std::max(verts[i].x, max.x);
		max.y = std::max(verts[i].y, max.y);
		max.z = std::max(verts[i].z, max.z);

		min.x = std::min(verts[i].x, min.x);
		min.y = std::min(verts[i].y, min.y);
		min.z = std::min(verts[i].z, min.z);
	}
}