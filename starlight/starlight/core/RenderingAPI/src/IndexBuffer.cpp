#pragma once
#include "VertexBuffer.h"
#include "Renderer.h"
#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(unsigned int* data, unsigned int count)
	:m_IndexCount(count)
{
	ASSERT(sizeof(GLuint) == sizeof(unsigned int));
	GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	// stack allocating the IndexBuffer results in an infinite loop because
	// the OpenGL context is terminated before this destructor is called.
	// glGetError will raise an error if there is no valid OpenGL context
	// available, so there will be an infinite loop of errors coming in.
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
