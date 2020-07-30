#include "RenderObj.h"
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"

RenderObj::RenderObj(Material* mat, Mesh* mesh) : m_mat(mat), m_mesh(mesh) {

}

RenderObj::~RenderObj() {
	delete m_mesh;
	delete m_mat;
}


void RenderObj::Update(float deltaTime) {
	Vector3 translation = transform.GetTranslation();
	angle += Math::Pi * rotSpeed;
	transform = Matrix4::CreateTranslation(translation) * Matrix4::CreateRotationY(angle);
	/*position -= Vector3::UnitZ * deltaTime;
	transform = Matrix4::CreateTranslation(position) * Matrix4::CreateRotationY(Math::ToRadians(180.f));*/
}