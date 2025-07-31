#pragma once

#include <chrono>

namespace Nigozi
{
	namespace Test
	{
		class ScopedTimer {
		public:
			ScopedTimer();
			~ScopedTimer();
		private:
			std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimePoint;
		};

		class Timer {
		public:
			Timer() = default;
			~Timer() = default;

			float StartTimerAndReturnSeconds();
			float EndTimerAndReturnSeconds();
		private:
			std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimePoint;
		};
	}
}