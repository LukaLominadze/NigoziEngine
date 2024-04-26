#pragma once

#ifdef NG_PLATFORM_WINDOWS
	#ifdef NG_SHARED_LIB
		#ifdef NG_BUILD_DLL
			#define NG_API __declspec(dllexport)
		#else
			#define NG_API __declspec(dllimport)
		#endif
	#else
		#define NG_API
	#endif
#endif