#pragma once
#include <GLAD/include/glad.h>
// if assertion fails, break at the current line
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError(); x; ASSERT(GLLogError(#x, __FILE__, __LINE__))
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"


void GLClearError();

bool GLLogError(const char* function, const char* file, int line);

class Renderer
{
public:
	static void Clear();
	static void Draw(const VertexArray& va, unsigned int numIndices, const IndexBuffer& ib, const Shader& shader);
};

