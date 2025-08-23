#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#ifndef DISTRIBUTION
// Deprecated
#define LOG(x) std::cout << x << std::endl
// Deprecated
#define ERROR_LOG(x) LOG(x);
#define ASSERT(x, message) if ((x)) LOG(message << " Success! " << __FILE__ << "; " << __LINE__ << "; " << #x); \
								else LOG(message << " Failed! " << __FILE__ << "; " << __LINE__ << "; " << #x);
#define ASSERT_ERROR(x) if (!(x)) LOG("Failed! " << __FILE__ << "; " << __LINE__ << "; " << #x);
#define GLCall(x) x; ASSERT_ERROR(GLLogCall(#x, __FILE__, __LINE__));

inline void GLClearError() {
    while (GLenum error = glGetError());
}

inline bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "{OpenGL Error} (" << error << "): " << function << " " << file << std::endl;
        return false;
    }
    return true;
}
#else
#define LOG(x)
#define ERROR_LOG(x)
#define ASSERT(x, message)
#define GLCall(x) x;
#endif