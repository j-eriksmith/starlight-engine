#include "DefaultModel.h"
#include "Texture.h"
#include "Renderer.h"

DefaultModel::DefaultModel(ShapeLoader::ShapeType m, std::string texturePath)
	:vertices(ShapeLoader::LoadVertexData(m)),
	indices(ShapeLoader::LoadIndexData(m)),
	VBO((void*)& vertices[0], vertices.size() * sizeof(Vertex)),
	IBO(&indices[0], indices.size())
{
	if (texturePath.size())
		LoadTexture(texturePath);
	// Initialize the VertexBufferLayout object to have the proper
	// vertex attribs and the correct stride (stride should include the texture coordinates,
	// even if there is no vertex attrib for it.
	layout.Push<float>(3);
	layout.Push<float>(3);
	//layout.AddToStride(3 * sizeof(float));
	layout.AddToStride(2 * sizeof(float));
	IBO.Bind();
	// After formatting our layout, we can now add our Vertex Buffer Data to our VAO.
	VAO.AddBuffer(VBO, layout);
}

void DefaultModel::Draw(Shader& shader)
{
	// std::cout << "indices size: " << indices.size() << std::endl;
	// Activate a new texture unit that we will bind our texture object to
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	glActiveTexture(GL_TEXTURE0);

	std::string number;
	std::string name = texture.type;

	if (name == "texture_diffuse")
	{
		number = std::to_string(diffuseNr++);
	}
	else if (name == "texture_specular")
	{
		number = std::to_string(specularNr++);
	}
	//Bind each Texture to a uniform in the given shader
	shader.SetUniform1i(name + number, 0);
	GLCall(glBindTexture(GL_TEXTURE_2D, texture.id));

	Renderer::Draw(VAO, indices.size(), IBO, shader);
}

void DefaultModel::LoadTexture(std::string texturePath)
{
	// use 'AddAttribPtr' when enabling the texture coords
	layout.AddAttribPtr<float>(2);
	// Load the texture into memory from local file
	texture = Texture(texturePath);
}

void DefaultModel::PrintIndices()
{
	int count = 0;
	for (int i = 0; i < indices.size(); ++i)
	{
		count++;
		std::cout << indices[i] << ", ";
		if (count % 3 == 0)
		{
			std::cout << std::endl;
			count = 0;
		}
	}
	std::cout << std::endl;
}

void DefaultModel::PrintVertices()
{
	for (const Vertex& v : vertices)
	{
		std::cout << v << std::endl;
	}
		
}