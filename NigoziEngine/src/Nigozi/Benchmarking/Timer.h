#pragma once

#include <chrono>

namespace Nigozi
{
	namespace Test
	{
		class ScopedTimer {
		public:
			[[nodiscard]]
			ScopedTimer();
			~ScopedTimer();
		private:
			std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimePoint;
		};

		class Timer {
		public:
			[[nodiscard]]
			Timer() = default;
			~Timer() = default;

			float StartTimerAndReturnSeconds();
			float EndTimerAndReturnSeconds();
		private:
			std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimePoint;
		};
	}
}