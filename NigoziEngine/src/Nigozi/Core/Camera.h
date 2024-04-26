#pragma once

#include "ngpch.h"
#include "Nigozi/Core/Core.h"

namespace Nigozi
{
	namespace Camera {
		inline Vector2<float> Position(0, 0);
		inline unsigned int WindowHeight;
		inline float OrthographicSize = 5;
		inline float UnitSize;

		inline void SetOrthograpicSize(float newSize) {
			OrthographicSize = newSize;
			UnitSize = (float)WindowHeight / OrthographicSize;
		}
		inline void IncrOrthograpicSize(float size) {
			OrthographicSize += size;
			UnitSize = (float)WindowHeight / OrthographicSize; 
		}
	}
}

