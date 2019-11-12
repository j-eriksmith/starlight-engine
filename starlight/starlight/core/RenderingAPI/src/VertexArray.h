#pragma once
#include "VertexBuffer.h"

class VertexBufferLayout;

class VertexArray
{
	// functionality:
	// 1. Bind our vertex array
	// 2. Bind a vertex buffer to a specific index in our vertex array
	// 3. Bind a layout to the buffer located at a specific index

private:
	unsigned int m_RendererID;

public:
	VertexArray();

	~VertexArray();

	void Bind() const;

	void Unbind() const;

	void AddBuffer(VertexBuffer& vb, VertexBufferLayout& layout);

};