#include "ngpch.h"
#include "Material.h"

namespace Nigozi
{
	Material::Material(Ref<Shader>& shader)
	{
		p_shader = shader;
	}

	Material::Material(Material&& other)
	{
		p_shader = other.p_shader;
	}

	Material::~Material()
	{
	}
}
