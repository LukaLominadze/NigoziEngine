#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#ifdef NG_PLATFORM_WINDOWS
	#define ASSERT_ERROR(x) \
				if (!(x)){ \
					std::cerr << "Failed!" << std::endl; \
					__debugbreak(); \
				}
	#ifdef DISTRIBUTION
		#define LOG(message)
		#define ERROR_LOG(message) __debugbreak()
	#else
		#define LOG(message) std::cout << message << std::endl;
		#define ERROR_LOG(message) \
			std::cout << message << std::endl; \
			__debugbreak()
	#endif
	#ifdef _DEBUG
		#define GLCall(x) x; ASSERT_ERROR(GLLogCall(#x, __FILE__, __LINE__))
		#define DEBUG_LOG(message) \
			std::cout << message << std::endl
		#define ASSERT(x, message) \
					std::cerr << message; \
					if ((x)){ \
						std::cerr << " -> Success!" << std::endl; \
					} \
					else { \
						std::cerr << " -> Failed!" << std::endl; \
						__debugbreak(); \
					}
	#else
		#define DEBUG_LOG(message)
		#define ASSERT(x, message)
		#define GLCall(x) x;
	#endif
#endif

#ifdef _DEBUG
inline bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "{OpenGL Error} (" << error << "): " << function << " " << file << std::endl;
        return false;
    }
    return true;
}
#endif