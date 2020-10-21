#include "stdafx.h"
#include "RenderObj.h"
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"

RenderObj::RenderObj(Ref<Material> mat, Ref<Mesh> mesh)
	: m_mat(mat)
	, m_mesh(mesh)
	, bounds(mesh.get())
{

}

void RenderObj::Update(float deltaTime) {
	bounds.Update();
}