#pragma once
#include "ngpch.h"
#include <windows.h>


#ifdef _DEBUG

	inline HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// Check if a task is succesfully completed
	#define ASSERT(condition, message) \
			if ((condition)){ \
				SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN); \
				std::cerr << message " -> Success!" << std::endl; \
			} \
			else { \
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED); \
				std::cerr << message << " -> Failed!" << std::endl; \
				__debugbreak(); \
			} \
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	#define LOG(message) \
				std::cout << message << std::endl;
#else
	#define ASSERT(condition, message)
	#define LOG(message)
#endif
