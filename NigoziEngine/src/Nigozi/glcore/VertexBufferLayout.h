#pragma once

#include "ngpch.h"

namespace Nigozi
{
	struct VertexBufferLayoutElement {
		uint32_t Type;
		uint32_t Count;
		char Normalized;

		static uint32_t GetTypeSize(uint32_t type) {
			switch (type) {
			case GL_FLOAT: return 4;
			case GL_UNSIGNED_INT: return 4;
			case GL_UNSIGNED_BYTE: return 1;
			}
			ERROR_LOG("Wrong type used on function : VertexBufferLayout::GetTypeSize(uint32_t type)");
			return 0;
		}
	};

	class VertexBufferLayout
	{
	public:
		template<typename T>
		void Push(uint32_t count) {
			ERROR_LOG("Wrong type used on function template : VertexBufferLayout::Push(uint32_t count)");
		}

		template<>
		void Push<float>(uint32_t count) {
			m_elements.push_back(VertexBufferLayoutElement{ GL_FLOAT, count, GL_FALSE });
			m_stride += VertexBufferLayoutElement::GetTypeSize(GL_FLOAT) * count;
		}

		template<>
		void Push<uint32_t>(uint32_t count) {
			m_elements.push_back(VertexBufferLayoutElement{ GL_UNSIGNED_INT, count, GL_FALSE });
			m_stride += VertexBufferLayoutElement::GetTypeSize(GL_UNSIGNED_INT) * count;
		}

		template<>
		void Push<unsigned char>(uint32_t count) {
			m_elements.push_back(VertexBufferLayoutElement{ GL_UNSIGNED_BYTE, count, GL_TRUE });
			m_stride += VertexBufferLayoutElement::GetTypeSize(GL_UNSIGNED_BYTE) * count;
		}

		inline const std::vector<VertexBufferLayoutElement>& GetElements() const { return m_elements; }
		inline uint32_t GetStride() const { return m_stride; }
	private:
		std::vector<VertexBufferLayoutElement> m_elements;
		uint32_t m_stride = 0;

	};
}
