#pragma once
#include "Geometry.h"
class Mesh;
class Texture;
class Material;

class RenderObj {
public:
	RenderObj(Ref<Material> mat, Ref<Mesh> mesh);
	~RenderObj() = default;

	void Update(float deltaTime);
	Mesh* GetMesh() { return m_mesh.get(); }
	Material* GetMat() { return m_mat.get();  }

	Matrix4 transform = Matrix4::Identity;

	float rotSpeed = 0.01f;
	AABB bounds;
private:
	float angle = 0.0f;
	Vector3 position = Vector3::Zero;

	Ref<Material> m_mat;
	Ref<Mesh> m_mesh;
};