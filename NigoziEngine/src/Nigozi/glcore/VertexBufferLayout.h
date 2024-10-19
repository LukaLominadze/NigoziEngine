#pragma once

#include "ngpch.h"

namespace Nigozi
{
	struct VertexBufferLayoutElement {
		unsigned int Type;
		unsigned int Count;
		char Normalized;

		static unsigned int GetTypeSize(unsigned int type) {
			switch (type) {
			case GL_FLOAT: return 4;
			case GL_UNSIGNED_INT: return 4;
			case GL_UNSIGNED_BYTE: return 1;
			}
			ERROR_LOG("Wrong type used on function : VertexBufferLayout::GetTypeSize(unsigned int type)");
			return 0;
		}
	};

	class VertexBufferLayout
	{
	public:
		template<typename T>
		void Push(unsigned int count) {
			ERROR_LOG("Wrong type used on function template : VertexBufferLayout::Push(unsigned int count)");
		}

		template<>
		void Push<float>(unsigned int count) {
			m_elements.push_back(VertexBufferLayoutElement{ GL_FLOAT, count, GL_FALSE });
			m_stride += VertexBufferLayoutElement::GetTypeSize(GL_FLOAT) * count;
		}

		template<>
		void Push<unsigned int>(unsigned int count) {
			m_elements.push_back(VertexBufferLayoutElement{ GL_UNSIGNED_INT, count, GL_FALSE });
			m_stride += VertexBufferLayoutElement::GetTypeSize(GL_UNSIGNED_INT) * count;
		}

		template<>
		void Push<unsigned char>(unsigned int count) {
			m_elements.push_back(VertexBufferLayoutElement{ GL_UNSIGNED_BYTE, count, GL_TRUE });
			m_stride += VertexBufferLayoutElement::GetTypeSize(GL_UNSIGNED_BYTE) * count;
		}

		inline const std::vector<VertexBufferLayoutElement>& GetElements() const { return m_elements; }
		inline unsigned int GetStride() const { return m_stride; }
	private:
		std::vector<VertexBufferLayoutElement> m_elements;
		unsigned int m_stride;

	};
}
