#pragma once

#include "ngpch.h"
#include "Shader.h"

namespace Nigozi
{
	class Material
	{
	public:
		Material(std::shared_ptr<Shader>& shader);
		~Material();

		const std::shared_ptr<Shader>& GetShader() const { return p_shader; }

	private:
		void ImportShaderParameterInfo();

		std::shared_ptr<Shader> p_shader;
		std::unordered_map<std::string, std::any> m_parameters;
	};
}