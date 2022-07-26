#include "pch.h"
#include "Material.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"

ATLAT::Material::Material()
{
	_data = std::make_shared<MaterialData>();
	
}

void ATLAT::Material::SetShader(std::shared_ptr<ATLAT::Shader> shader)
{
	Renderer::GetInstance().GetRenderer()->SetShader(GetUID(), shader->GetUID());
}
void ATLAT::Material::SetTexture(std::shared_ptr<ATLAT::Texture> texture)
{
	Renderer::GetInstance().GetRenderer()->SetTexture(GetUID(), texture->GetUID());
}

void ATLAT::Material::SetData(const MaterialData& data)
{
	*_data = data;
	PushData();
}

void ATLAT::Material::PushData()
{
	Renderer::GetInstance().GetRenderer()->SetMaterialData(_data.get(), sizeof(*_data), GetUID());
}
