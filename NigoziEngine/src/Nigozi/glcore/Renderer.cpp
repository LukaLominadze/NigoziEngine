#include "ngpch.h"

#include "Renderer.h"

namespace Nigozi
{
	Renderer::Renderer()
	{
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::SetClearColor(float v0, float v1, float v2, float v3)
	{
		GLCall(glClearColor(v0, v1, v2, v3));
	}

	void Renderer::Clear()
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}

	void Renderer::SetDrawColor(Shader& shader, const char* uniform, float r, float g, float b, float a)
	{
		shader.Bind();
		shader.SetUniform4f(uniform, r, g, b, a);
		shader.Unbind();
	}

	void Renderer::Draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) const
	{
		vao.Bind();
		ibo.Bind();
		shader.Bind();
		GLCall(glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr));
	}
}
