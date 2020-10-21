#include "stdafx.h"
#include "RenderObj.h"
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"

RenderObj::RenderObj(Material* mat, Mesh* mesh) 
	: m_mat(mat)
	, m_mesh(mesh)
	, bounds(mesh)
{

}

RenderObj::~RenderObj() {
	delete m_mesh;
	delete m_mat;
}


void RenderObj::Update(float deltaTime) {
	bounds.Update();
	/*
	Vector3 translation = transform.GetTranslation();
	angle += Math::Pi * rotSpeed;
	transform = Matrix4::CreateTranslation(translation) * Matrix4::CreateRotationY(angle); */
}