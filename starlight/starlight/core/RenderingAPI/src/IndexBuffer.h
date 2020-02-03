#pragma once

class IndexBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_IndexCount;

public:
	IndexBuffer(unsigned int* data, unsigned int count);

	IndexBuffer();

	~IndexBuffer();

	void Bind() const;

	void Unbind() const;

	unsigned int GetCount() const { return m_IndexCount; };
};