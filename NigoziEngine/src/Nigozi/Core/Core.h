#pragma once

#ifdef NG_PLATFORM_WINDOWS
	#ifdef NG_BUILD_DLL
		#define NG_API __declspec(dllexport)
	#else
		#define NG_API __declspec(dllimport)
	#endif
#endif