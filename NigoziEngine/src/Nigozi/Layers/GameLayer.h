#pragma once
#include "Layer.h"

namespace Nigozi
{
	template<typename T>
	class GameLayer : public Layer {
	public:
		virtual void PushToVector(T* obj) = 0;
		virtual void PopFromVector(T* obj) = 0;

		inline void SetLayerIndex(uint32_t layerIndex) { m_layerIndex = layerIndex; };
	protected:
		uint32_t m_layerIndex;
	};

	#define BUILD_GET_INDEX_FUNC static void SetGetIndexFunc(std::function<uint32_t()> func) { \
									GetInsertIndex = func; \
								 } \
								 static std::function<uint32_t()> GetInsertIndex;
}