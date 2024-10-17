#pragma once

#include "VertexArray.h";
#include "IndexBuffer.h"
#include "Shader.h"

namespace Nigozi
{
	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		void SetClearColor(float v0, float v1, float v2, float v3);
		void Clear();
		void SetDrawColor(Shader& shader, const char* uniform, float r, float g, float b, float a);
		void Draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) const;
	};
}

