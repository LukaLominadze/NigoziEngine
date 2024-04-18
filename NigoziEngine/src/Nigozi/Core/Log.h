#pragma once
#include "ngpch.h"

#ifdef _DEBUG

	// Check if a task is succesfully completed
	#define ASSERT(condition, message) \
			if ((condition)){ \
				std::cerr << message " -> Success!" << std::endl; \
			} \
			else { \
				std::cerr << message << " -> Failed!" << std::endl; \
				__debugbreak(); \
			}
	#define LOG(message) \
				std::cout << message << std::endl;
#else
	#define ASSERT(condition, message) ((void)0)
	#define LOG(message) ((void)0)
#endif
