#pragma once

#include "ngpch.h"
#include "Nigozi/Core/Core.h"

namespace Nigozi
{
	class NG_API Layer {
	public:
		virtual void SetIndex(uint32_t layerIndex) {}
		virtual void OnAttach() {}
		virtual void OnUpdate() {}
		virtual void OnRender() {}
		virtual void OnDetach() {}
	protected:
		uint32_t m_layerIndex = 0;
	};

#define BUILD_GET_INDEX_FUNC static void SetGetIndexFunc(std::function<uint32_t()> func) { \
								GetInsertIndex = func; \
							 } \
							 static std::function<uint32_t()> GetInsertIndex;

#define SET_INDEX(this_class) void this_class::SetIndex(uint32_t layerIndex) \
						      {  \
						      	  m_layerIndex = layerIndex; \
						      	  SetGetIndexFunc([&]() { return m_layerIndex; }); \
						      }  
}