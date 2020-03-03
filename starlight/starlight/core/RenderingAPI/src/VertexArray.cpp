#include "VertexArray.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"
#include <iostream>

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}
void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}

void VertexArray::AddBuffer(VertexBuffer& vb, VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const std::vector<LayoutElement>& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); ++i)
	{
		const LayoutElement& element = elements[i];
		// enables attribute 0 for our currently bound vertex array
		// specifies attribute 0 for our currently bound vertex array
		// this line of code is what binds our vertex buffer to our vertex array, because we specify
		// that we want our currently bound buffer to be used to render the current object.
		// glVertexAttribPointer parameter specification:
		// 1. Index: The index of the attribute we want to configure. This could be the position, normal vectors, texture coords, etc.
		// 2. Size: How many components make up this attribute per vertex? Ex. Position in a 3D world is represented using 3 floats, the the size would be 3.
		// 3. Type: The type of component you use to represent your attribute
		// 4. Normalized: boolean value - are the values that represent your attribute normalized?
		// 5. Stride: How large (in bytes) is each vertex? In other words, how much space is there between each vertex?
		// 6. Within your array of vertex data, where does the data for this attribute start?
		//! @todo: Enable users to specify which attributes they do and do not want to include in the VertexAttribArray
		//! Ex. vertex array has position, normal, and texcoord attributes, but user only wants to use position and texcoords.
		//! Must be able to customize the offset value depending on which attributes you want to include, and also specify the 
		//! correct stride value as well.
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
		GLCall(glEnableVertexAttribArray(i));
		offset += element.count * LayoutElement::GetSizeOfType(element.type);
	}

}
