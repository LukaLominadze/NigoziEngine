#pragma once

#include "ngpch.h"
#include "Shader.h"
#include "ref/Ref.h"

namespace Nigozi
{
	class Material
	{
	public:
		Material(Ref<Shader>& shader);
		Material(Material&& other);
		~Material();

		const Ref<Shader> GetShader() const { return p_shader; }

	private:
		Ref<Shader> p_shader;
		std::unordered_map<std::string, std::any> m_parameters;
	};
}