#include "ngpch.h"
#include "Material.h"

Nigozi::Material::Material(std::shared_ptr<Shader>& shader)
{
	p_shader = shader;
}

Nigozi::Material::~Material()
{
}
