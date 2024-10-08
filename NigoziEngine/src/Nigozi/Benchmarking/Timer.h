#pragma once

#include <chrono>

namespace Nigozi
{
	class Timer {
	public:
		Timer();
		~Timer();
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimePoint;
	};
}