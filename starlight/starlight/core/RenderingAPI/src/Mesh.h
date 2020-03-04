#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Texture.h"
#include "Vertex.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"


class Mesh
{
public:

	Mesh(std::vector<Vertex> v, std::vector<unsigned int> i, std::vector<std::shared_ptr<Texture>> t);

	Mesh(const Mesh& lhs);
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<std::shared_ptr<Texture>> textures;

	VertexArray vao;
	VertexBuffer vbo;
	VertexBufferLayout layout;
	IndexBuffer ibo;

	void SetupMesh();

	void Unbind();
};