#pragma once
#include "Nigozi/Components/Component.h"

#include "ngpch.h"

namespace Nigozi
{
	class NG_API BoxCollider : public Component
	{
	public:
		BoxCollider() = default;
		~BoxCollider() = default;

		void SetDimensions();

		const SDL_Rect& GetRect() const { return m_rect; }

		BUILD_C_GET_INDEX_FUNC

		static const char* GetType() {
			return "BoxCollider";
		}
	private:
		SDL_Rect m_rect;
	};
}
