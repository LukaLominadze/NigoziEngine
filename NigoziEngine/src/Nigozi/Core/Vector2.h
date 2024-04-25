#pragma once

#include "Nigozi/Core/Core.h"

namespace Nigozi
{
	template<typename T>
	class NG_API Vector2 {
	public:
		Vector2() = default;
		inline Vector2(T x, T y) {
			this->x = x;
			this->y = y;
		}
		~Vector2() = default;

		Vector2 operator+(const Vector2& other) const {
			return Vector2(x + other.x, y + other.y);
		}

		Vector2& operator+=(const Vector2& other) {
			x += other.x;
			y += other.y;
			return *this;
		}

		Vector2 operator*(const Vector2& other) const {
			return Vector2(x * other.x, y * other.y);
		}

		Vector2 operator*(const T& other) const {
			return Vector2(x * other, y * other);
		}

		T x, y;
	};
}