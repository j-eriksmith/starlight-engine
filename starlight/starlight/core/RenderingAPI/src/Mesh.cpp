#include "Mesh.h"
#include <GLAD/include/glad.h>
#include <iostream>

Mesh::Mesh(std::vector<Vertex> v, std::vector<unsigned int> i, std::vector<std::shared_ptr<Texture>> t)
	:vertices(v), 
	indices(i), 
	textures(t),
	vbo((void*)&vertices[0], vertices.size() * sizeof(Vertex)),
	ibo(&indices[0], indices.size())
{
	SetupMesh();
}

//void Mesh::BindTextures(ShaderComponent& shader)
//{
//	unsigned int diffuseNr = 1;
//	unsigned int specularNr = 1;
//	// Bind each of our mesh's textures to uniforms in our shader and use
//	// them to draw our mesh
//	for (unsigned int i = 0; i < textures.size(); ++i)
//	{
//		// Activate a new texture unit that we will bind our texture object to
//		glActiveTexture(GL_TEXTURE0 + i);
//
//		std::string number;
//		std::string name = textures[i].type;
//
//		if (name == "texture_diffuse")
//		{
//			number = std::to_string(diffuseNr++);
//		}
//		else if (name == "texture_specular")
//		{
//			number = std::to_string(specularNr++);
//		}
//		//Bind each Texture to a uniform in the given shader
//		ShaderSystem::SetUniform1i(shader, name + number, i);
//		GLCall(glBindTexture(GL_TEXTURE_2D, textures[i].id));
//	}
//	// Reset our active texture back to the default
//	glActiveTexture(GL_TEXTURE0);
//
//	// Bind our vertex array object, which contains our vertex buffer object 
//	// that was bound to our VAO during our SetupMesh fn call
//	// Renderer::Draw(vao, indices.size(), ibo, shader);
//	Unbind();
//}
//void Mesh::Draw(Shader& shader)
//{
//	unsigned int diffuseNr = 1;
//	unsigned int specularNr = 1;
//	// Bind each of our mesh's textures to uniforms in our shader and use
//	// them to draw our mesh
//	for (unsigned int i = 0; i < textures.size(); ++i)
//	{
//		// Activate a new texture unit that we will bind our texture object to
//		glActiveTexture(GL_TEXTURE0 + i);
//
//		std::string number;
//		std::string name = textures[i].type;
//
//		if (name == "texture_diffuse")
//		{
//			number = std::to_string(diffuseNr++);
//		}
//		else if (name == "texture_specular")
//		{
//			number = std::to_string(specularNr++);
//		}
//		//Bind each Texture to a uniform in the given shader
//		shader.SetUniform1i(name + number, i);
//		GLCall(glBindTexture(GL_TEXTURE_2D, textures[i].id));
//	}
//	// Reset our active texture back to the default
//	glActiveTexture(GL_TEXTURE0);
//
//	// Bind our vertex array object, which contains our vertex buffer object 
//	// that was bound to our VAO during our SetupMesh fn call
//	Renderer::Draw(vao, indices.size(), ibo, shader);
//	Unbind();
//}
void Mesh::SetupMesh()
{
	ibo.Bind();
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<float>(2);
	// Bind VBO / layout to VAO
	vao.AddBuffer(vbo, layout);

	Unbind();
}

void Mesh::Unbind()
{
	vao.Unbind();
	ibo.Unbind();
}