#include "ngpch.h"

#include "Renderer2D.h"

namespace Nigozi
{
	Renderer2DData* Renderer2D::s_data = nullptr;

	/// <summary>
	/// Sets OpenGL parameters, intializes rendererData: Shader, Texture array, VAO, VBO, IBO
	/// </summary>
	void Renderer2D::Initialize()
	{
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		/*GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glDepthFunc(GL_LESS));*/

		LOG("OpenGL Info: ");
		LOG("  Vendor: {0} " << glGetString(GL_VENDOR));
		LOG("  Renderer: {0} " << glGetString(GL_RENDERER));
		LOG("  Version: {0} " << glGetString(GL_VERSION));

		// Create renderer data object
		s_data = new Renderer2DData();

		// Create VAO
		s_data->QuadVertexArray = new VertexArray();
		std::shared_ptr<VertexBuffer> vbo = std::make_shared<VertexBuffer>(
			s_data->Vertices,
			s_data->MaxVerticesElements * sizeof(float)
		);
		// Set up vertex buffer and it's layout, set up index buffer and bind to vao
		VertexBufferLayout layout;
		layout.Push<float>(3); // position
		layout.Push<float>(2); // texCoords
		layout.Push<float>(4); // color
		layout.Push<float>(1); // textureSlot

		int offset = 0;
		for (int i = 0; i < s_data->MaxIndicies; i += 6) {
			s_data->Indicies[i + 0] = offset + 0;
			s_data->Indicies[i + 1] = offset + 1;
			s_data->Indicies[i + 2] = offset + 2;

			s_data->Indicies[i + 3] = offset + 2;
			s_data->Indicies[i + 4] = offset + 3;
			s_data->Indicies[i + 5] = offset + 0;

			offset += 4;
		}

		std::shared_ptr<IndexBuffer> ibo = std::make_shared<IndexBuffer>(
			s_data->Indicies,
			s_data->MaxIndicies
		);

		s_data->QuadVertexArray->AddVertexBuffer(vbo, layout);
		s_data->QuadVertexArray->AddIndexBuffer(ibo);
		// Create shader and set up texture units
		s_data->DefaultShader = std::make_shared<Shader>("src/Nigozi/res/shaders/Default.glsl");
		
		for (int i = 0; i < 16; i++) 
		{
			s_data->TextureUnits[i] = i;
		}

		s_data->DefaultShader->SetUniform1iv("u_Textures", 16, s_data->TextureUnits);

		s_data->Textures[0] = std::make_shared<Texture>("src/Nigozi/res/textures/flatQuad.png");
		s_data->Textures[s_data->TextureSlot++]->Bind(0);
	}

	void Renderer2D::Deinitialize()
	{
		for (int i = 0; i < 16; i++) {
			if (s_data->Textures[i]) {
				s_data->Textures[i]->Delete();
				continue;
			}
			break;
		}
		s_data->QuadVertexArray->Delete();
		s_data->DefaultShader->Delete();
		delete s_data->QuadVertexArray;
		delete s_data;
	}

	void Renderer2D::SetClearColor(float v0, float v1, float v2, float v3)
	{
		GLCall(glClearColor(v0, v1, v2, v3));
	}

	void Renderer2D::Clear()
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& scale, std::shared_ptr<SubTexture>& texture, const glm::vec4& color) {
		DrawQuad(position, scale, texture->GetTexture(), color, texture->GetCoordMin(), texture->GetCoordMax());
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& scale, const std::shared_ptr<Texture>& texture, const glm::vec4& color, const glm::vec2& coordMin, const glm::vec2& coordMax)
	{
		FlushIfExceededMaxVertexCount();

		int textureSlot = SetTextureAndGetSlot(texture);

		glm::vec2 halfSize = scale / 2.0f;

		float quad[] = {
			position.x - halfSize.x, position.y - halfSize.y, position.z, coordMin.x, coordMin.y, color.x, color.y, color.z, color.w, textureSlot,
			position.x - halfSize.x, position.y + halfSize.y, position.z, coordMin.x, coordMax.y, color.x, color.y, color.z, color.w, textureSlot,
			position.x + halfSize.x, position.y + halfSize.y, position.z, coordMax.x, coordMax.y, color.x, color.y, color.z, color.w, textureSlot,
			position.x + halfSize.x, position.y - halfSize.y, position.z, coordMax.x, coordMin.y, color.x, color.y, color.z, color.w, textureSlot,
		};
		int vertexIndex = s_data->CurrentVertex / s_data->VertexElementCount;

		for (int i = s_data->CurrentVertex; i < s_data->CurrentVertex + 4 * s_data->VertexElementCount; i++) {
			s_data->Vertices[i] = quad[i - s_data->CurrentVertex];
		}

		s_data->CurrentVertex += 4 * s_data->VertexElementCount;
		s_data->CurrentIndex += 6;
		s_data->QuadCount++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& scale, float rotation, std::shared_ptr<SubTexture>& texture, const glm::vec4& color)
	{
		DrawRotatedQuad(position, scale, rotation, texture->GetTexture(), color, texture->GetCoordMin(), texture->GetCoordMax());
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& scale, float rotation, const std::shared_ptr<Texture>& texture, const glm::vec4& color, const glm::vec2& coordMin, const glm::vec2& coordMax)
	{
		FlushIfExceededMaxVertexCount();

		int textureSlot = SetTextureAndGetSlot(texture);

		glm::vec2 halfSize = scale / 2.0f;

		glm::mat2 rotationMatrix = glm::mat2(glm::cos(rotation), -glm::sin(rotation),
											 glm::sin(rotation), glm::cos(rotation));

		glm::vec2 downLeft = rotationMatrix * glm::vec2(-halfSize.x, -halfSize.y);
		glm::vec2 upLeft = rotationMatrix * glm::vec2(-halfSize.x, halfSize.y);
		glm::vec2 upRight = rotationMatrix * glm::vec2(halfSize.x, halfSize.y);
		glm::vec2 downRight = rotationMatrix * glm::vec2(halfSize.x, -halfSize.y);

		float quad[] = {
			position.x + downLeft.x, position.y + downLeft.y, position.z, coordMin.x, coordMin.y, color.x, color.y, color.z, color.w, textureSlot,
			position.x + upLeft.x, position.y + upLeft.y, position.z, coordMin.x, coordMax.y, color.x, color.y, color.z, color.w, textureSlot,
			position.x + upRight.x, position.y + upRight.y, position.z, coordMax.x, coordMax.y, color.x, color.y, color.z, color.w, textureSlot,
			position.x + downRight.x, position.y + downRight.y, position.z, coordMax.x, coordMin.y, color.x, color.y, color.z, color.w, textureSlot,
		};
		int vertexIndex = s_data->CurrentVertex / s_data->VertexElementCount;

		for (int i = s_data->CurrentVertex; i < s_data->CurrentVertex + 4 * s_data->VertexElementCount; i++) {
			s_data->Vertices[i] = quad[i - s_data->CurrentVertex];
		}

		s_data->CurrentVertex += 4 * s_data->VertexElementCount;
		s_data->CurrentIndex += 6;
		s_data->QuadCount++;
	}

	void Renderer2D::BeginScene()
	{
		s_data->DrawCalls = 0;
		s_data->QuadCount = 0;
	}

	void Renderer2D::EndScene()
	{
		Flush();
	}

	void Renderer2D::Flush()
	{
		s_data->QuadVertexArray->SetVertexBufferData(
			s_data->Vertices,
			s_data->CurrentVertex * sizeof(float)
		);
		for (int i = 0; i < s_data->TextureSlot; i++) {
			s_data->Textures[i]->Bind(i);
		}

		s_data->QuadVertexArray->Bind();
		GLCall(glDrawElements(GL_TRIANGLES, s_data->CurrentIndex, GL_UNSIGNED_INT, nullptr));

		s_data->CurrentVertex = 0;
		s_data->CurrentIndex = 0;
		s_data->TextureSlot = 1;
		s_data->DrawCalls++;
	}

	void Renderer2D::SetMVPMatrix(const glm::mat4& mvp)
	{
		s_data->DefaultShader->SetUniformMat4f("modelViewProjection", mvp);
	}

	uint32_t Renderer2D::SetTextureAndGetSlot(const std::shared_ptr<Texture>& texture)
	{
		if (texture) {
			if (s_data->TextureSlot > 16) {
				Flush();
			}
			s_data->Textures[s_data->TextureSlot] = texture;
			texture->Bind(s_data->TextureSlot);
			return s_data->TextureSlot++;
		}
		return 0;
	}

	void Renderer2D::FlushIfExceededMaxVertexCount()
	{
		if (s_data->CurrentVertex > s_data->MaxVerticesElements) {
			Flush();
		}
	}
}
