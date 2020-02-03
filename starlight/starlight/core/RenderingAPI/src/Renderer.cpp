#include "Renderer.h"
#include <iostream>

void GLClearError()
{
	// cycle through
	while (glGetError() != GL_NO_ERROR);
		
}

bool GLLogError(const char* function, const char* file, int line)
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

void Renderer::Clear()
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
	// Clear the depth buffer from previous frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Renderer::Draw(const VertexArray& va, unsigned int numIndices, const IndexBuffer& ib, const Shader& shader)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	shader.Bind();
	va.Bind();
	ib.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr));
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::DrawWireFrame(const VertexArray& va, unsigned int numIndices, const IndexBuffer& ib, const Shader& shader)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	Draw(va, numIndices, ib, shader);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}