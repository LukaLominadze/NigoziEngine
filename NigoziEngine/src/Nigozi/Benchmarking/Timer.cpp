#include "ngpch.h"
#include "Timer.h"

namespace Nigozi
{
	namespace Test
	{
        ScopedTimer::ScopedTimer()
        {
            m_startTimePoint = std::chrono::high_resolution_clock::now();
        }

        ScopedTimer::~ScopedTimer()
        {
            auto endTimePoint = std::chrono::high_resolution_clock::now();

            auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimePoint).time_since_epoch().count();
            auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

            auto duration = end - start;
            double ms = duration * 0.001;

            std::cout << "Finished! -> " << ms << "ms." << std::endl;
        }

        float Timer::StartTimerAndReturnSeconds()
        {
            m_startTimePoint = std::chrono::high_resolution_clock::now();
            return 0.0f;
        }

        float Timer::EndTimerAndReturnSeconds()
        {
            auto endTimePoint = std::chrono::high_resolution_clock::now();

            auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimePoint).time_since_epoch().count();
            auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

            auto duration = end - start;

            return (float)((duration * 0.001f) * 0.001f);
        }
	}
}
