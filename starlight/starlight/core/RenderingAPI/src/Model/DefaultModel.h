#pragma once
#include "Model.h"
#include "Shader.h"
#include "Vertex.h"
#include <vector>
#include "ShapeLoader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "ShapeLoader.h"
#include <string>

class DefaultModel : public Model
{
public:

	DefaultModel(ShapeLoader::ShapeType m, std::string texturePath = "");
	void Draw(Shader& shader);
	void LoadTexture(std::string texturePath);

	void PrintVertices();

	void PrintIndices();
	   
private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	Texture texture;
	VertexArray VAO;
	VertexBuffer VBO;
	IndexBuffer IBO;
	VertexBufferLayout layout;
};