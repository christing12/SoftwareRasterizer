#pragma once
#include "EngineMath.h"
class Mesh;
class Texture;
class Material;

class RenderObj {
public:
	RenderObj(Material* mat, Mesh* mesh);
	~RenderObj();

	void Update(float deltaTime);
	Mesh* GetMesh() { return m_mesh; }
	Material* GetMat() { return m_mat;  }

	Matrix4 transform = Matrix4::Identity;

	float rotSpeed = 0.01f;
private:
	float angle = 0.0f;
	Vector3 position = Vector3::Zero;

	Material* m_mat;
	Mesh* m_mesh;
};