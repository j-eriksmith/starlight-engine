#pragma once
#include <iostream>
#include <string>
#include <GLAD/include/glad.h>

constexpr bool DEBUG = false;
#define Log(x) if (DEBUG) std::cout << x << std::endl

inline void GLClearError();
inline bool GLLogError(const char* function, const char* file, int line);

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError(); x; ASSERT(GLLogError(#x, __FILE__, __LINE__))

inline void GLClearError()
{
	// cycle through
	while (glGetError() != GL_NO_ERROR);

}

inline bool GLLogError(const char* function, const char* file, int line)
{
	bool success(true);
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error]: (" << error << ")" << "from function " << function << "in file " << file <<
			"on line " << line << std::endl;
		success = false;
	}
	return success;
}