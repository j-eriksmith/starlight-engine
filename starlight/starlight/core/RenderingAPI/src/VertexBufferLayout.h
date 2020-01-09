#pragma once
#include <vector>
#include <GLAD/include/glad.h>
#include "Renderer.h"

struct LayoutElement
{
	// Holds all of the parameters needed to make a glVertexAttribPointer call,
	// which will bind our current buffer to the VertexArray and specify a layout for
	// our buffer
	unsigned int type;
	unsigned int count;
	unsigned int normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:
			return 4;
		case GL_UNSIGNED_INT:
			return 4;
		case GL_UNSIGNED_BYTE:
			return 1;
		}
		ASSERT(false);
		return 0;
	}
};
class VertexBufferLayout
{
private:
	std::vector<LayoutElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout()
		: m_Stride(0)
	{}


	// Decouple AttribPtr and Stride by extracting AddAttribPtr
	// from the Push function
	template<typename T>
	void AddAttribPtr(unsigned int count)
	{
		static_assert(false);
	}

	template<>
	void AddAttribPtr<float>(unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
	}
	
	template<typename T>
	void AddAttribPtrAndNormalize(unsigned int count)
	{
		static_assert(false);
	}

	template<>
	void AddAttribPtrAndNormalize<float>(unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_TRUE });
	}

	template<>
	void AddAttribPtr<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
	}

	template<>
	void AddAttribPtr<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_FALSE });
	}

	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(false);
	}

	template<>
	void Push<float>(unsigned int count)
	{
		AddAttribPtr<float>(count);
		m_Stride += LayoutElement::GetSizeOfType(GL_FLOAT) * count;
	}
	template<typename T>
	void PushAndNormalize(unsigned int count)
	{
		static_assert(false);
	}

	template<>
	void PushAndNormalize<float>(unsigned int count)
	{
		AddAttribPtrAndNormalize<float>(count);
		m_Stride += LayoutElement::GetSizeOfType(GL_FLOAT) * count;
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		AddAttribPtr<unsigned int>(count);
		m_Stride += LayoutElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		// why are char elements normalize while the other types aren't?
		AddAttribPtr<unsigned char>(count);
		m_Stride += LayoutElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
	}

	inline const std::vector<LayoutElement>& GetElements() const { return m_Elements; }

	inline unsigned int GetStride() const { return m_Stride; }

	void AddToStride(unsigned int newStride) { m_Stride += newStride; }
};