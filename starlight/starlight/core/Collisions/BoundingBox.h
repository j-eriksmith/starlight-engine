#pragma once
#include <limits>
#include <vector>
#include "vec3.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"


struct BoundingBox
{

	BoundingBox(Vector3 c, float x, float y, float z, std::vector<float> v, std::vector<unsigned int> i);
	BoundingBox();
	void UpdateCenter(float x, float y, float z);
	void Draw(Shader& shader) const;
	bool HasCollided(const BoundingBox& rhs) const;
	Vector3 center;
	float radiusX;
	float radiusY;
	float radiusZ;
private:
	VertexBuffer vbo;
	VertexBufferLayout layout;
	VertexArray vao;
	IndexBuffer ibo;
};

struct BoundingBoxPrimitives
{
	float maxX;
	float maxY;
	float maxZ;
	float minX;
	float minY;
	float minZ;

	BoundingBoxPrimitives();
	void Print() const;
};