#pragma once
#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(void* data, unsigned int size)
{
	// generates a buffer, assigns it an id, and stores the id in 'buf'
	GLCall(glGenBuffers(1, &m_RendererID));
	// binds our id to an actual array buffer
	// by binding our buffer, we are selecting this buffer to be used when rendering whatever we want to render.
	// Now, if we unbind our vertex array and vertex buffer, when we rebind our vertex array, our vertex buffer
	// will still be attached to the vertex array, so we won't need to rebind it.
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
