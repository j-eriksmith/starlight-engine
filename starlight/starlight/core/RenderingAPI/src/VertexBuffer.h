#pragma once
#include <GLAD/include/glad.h>

class VertexBuffer
{
private:
	// id of the buffer we will use to render our item
	unsigned int m_RendererID;

public:
	VertexBuffer(void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};

