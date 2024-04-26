#pragma once

#include "Nigozi/Core/Core.h"
#include "Nigozi/Components/Transform.h"

#include "ngpch.h"

namespace Nigozi
{
	class NG_API Component {
	public:
		virtual const std::type_info& GetType() const {
			return typeid(*this);
		}

		inline void SetTransform(Transform* transform) { p_transform = transform; }
		inline Transform* GetTransform() const { return p_transform; }
	private:
		Transform* p_transform;
	};

#define BUILD_C_GET_INDEX_FUNC static uint32_t GetLayerIndex(); 

#define IMPL_C_GET_INDEX_FUNC(this_class, target_class)  uint32_t this_class::GetLayerIndex() { \
													       return target_class::GetInsertIndex(); \
												       }
}
