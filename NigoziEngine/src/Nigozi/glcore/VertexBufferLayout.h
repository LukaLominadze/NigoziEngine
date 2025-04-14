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
			unsigned int type;
			char normalized;
			if (std::is_same<T, float>::value) {
				type = GL_FLOAT;
				normalized = GL_FALSE;
			}
			else if (std::is_same<T, unsigned int>::value) {
				type = GL_UNSIGNED_INT;
				normalized = GL_FALSE;
			}
			else if (std::is_same<T, unsigned char>::value) {
				type = GL_UNSIGNED_BYTE;
				normalized = GL_TRUE;
			}
			else {
				ERROR_LOG("Wrong type used on function template : VertexBufferLayout::Push(uint32_t count)");
			}

			m_elements.push_back(VertexBufferLayoutElement{ type, count, normalized });
			m_stride += VertexBufferLayoutElement::GetTypeSize(type) * count;
		}

		inline const std::vector<VertexBufferLayoutElement>& GetElements() const { return m_elements; }
		inline uint32_t GetStride() const { return m_stride; }
	private:
		std::vector<VertexBufferLayoutElement> m_elements;
		uint32_t m_stride = 0;

	};
}
