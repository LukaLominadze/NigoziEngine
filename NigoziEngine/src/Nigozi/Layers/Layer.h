#pragma once

#include "ngpch.h"

namespace Nigozi
{
	class Layer {
	public:
		virtual void SetIndex(uint32_t layerIndex) = 0;
		virtual void OnAttach() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnDetach() = 0;
	};

#define BUILD_GET_INDEX_FUNC static void SetGetIndexFunc(std::function<uint32_t()> func) { \
								GetInsertIndex = func; \
							 } \
							 static std::function<uint32_t()> GetInsertIndex;
}