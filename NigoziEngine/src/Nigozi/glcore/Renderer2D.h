#pragma once

#include "VertexArray.h";
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

namespace Nigozi
{
	struct Renderer2DData 
	{
		static const unsigned int MaxQuads = 10'000;
		static const unsigned int MaxVertices = MaxQuads * 4;
		static const unsigned int VertexElementCount = 10;
		static const unsigned int MaxVerticesElements = MaxVertices * VertexElementCount;
		static const unsigned int MaxIndicies = MaxQuads * 6;

		float Vertices[MaxVerticesElements];
		unsigned int Indicies[MaxIndicies];

		unsigned int CurrentVertex = 0;
		unsigned int CurrentIndex = 0;

		VertexArray* QuadVertexArray;
		std::shared_ptr<Shader> DefaultShader;

		std::shared_ptr<Texture> Textures[16];
		unsigned int TextureSlot = 0;
		int TextureUnits[16];
	};

	class Renderer2D
	{
	public:
		static void Initialize();
		static void Deinitialize();

		static void SetClearColor(float v0, float v1, float v2, float v3);
		static void Clear();

		static void DrawQuad(const glm::vec3& position, const glm::vec2& scale, const std::shared_ptr<Texture>& texture, glm::vec4 color);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& scale, float rotation, const std::shared_ptr<Texture>& texture, glm::vec4 color);

		static void BeginScene();
		static void EndScene();
		static void Flush();

		static void SetMVPMatrix(const glm::mat4& mvp);

		static Renderer2DData* GetData() { return s_data; }
	private:
		static unsigned int SetTextureAndGetSlot(const std::shared_ptr<Texture>& texture);
		static void FlushIfExceededMaxVertexCount();
	private:
		static Renderer2DData* s_data;
	};
}

