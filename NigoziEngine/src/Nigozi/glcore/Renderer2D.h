#pragma once

#include "VertexArray.h";
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "SubTexture.h"

namespace Nigozi
{
	struct Renderer2DData 
	{
		static const uint32_t MaxQuads = 10'000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t VertexElementCount = 10;
		static const uint32_t MaxVerticesElements = MaxVertices * VertexElementCount;
		static const uint32_t MaxIndicies = MaxQuads * 6;

		float Vertices[MaxVerticesElements];
		uint32_t Indicies[MaxIndicies];

		uint32_t CurrentVertex = 0;
		uint32_t CurrentIndex = 0;
		uint32_t DrawCalls = 0;
		uint32_t QuadCount = 0;

		VertexArray* QuadVertexArray;
		std::shared_ptr<Shader> DefaultShader;

		std::shared_ptr<Texture> Textures[16];
		uint32_t TextureSlot = 0;
		int TextureUnits[16];
	};

	class Renderer2D
	{
	public:
		static void Initialize();
		static void Deinitialize();

		static void SetClearColor(float v0, float v1, float v2, float v3);
		static void Clear();

		static void DrawQuad(const glm::vec3& position, const glm::vec2& scale,
							 const std::shared_ptr<Texture>& texture, const glm::vec4& color,
							 const glm::vec2& coordMin = glm::vec2(0),
							 const glm::vec2& coordMax = glm::vec2(1));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& scale,
							 std::shared_ptr<SubTexture>& texture, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& scale,
									float rotation, const std::shared_ptr<Texture>& texture,
									const glm::vec4& color,
									const glm::vec2& coordMin = glm::vec2(0),
									const glm::vec2 & coordMax = glm::vec2(1));
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& scale,
									float rotation, std::shared_ptr<SubTexture>& texture,
									const glm::vec4& color);
		static void BeginScene();
		static void EndScene();
		static void Flush();

		static void SetMVPMatrix(const glm::mat4& mvp);

		static Renderer2DData* GetData() { return s_data; }
	private:
		static uint32_t SetTextureAndGetSlot(const std::shared_ptr<Texture>& texture);
		static void FlushIfExceededMaxVertexCount();
	private:
		static Renderer2DData* s_data;
	};
}

