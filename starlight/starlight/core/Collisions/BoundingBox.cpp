#include "BoundingBox.h"
#include "iostream"

constexpr double DOUBLE_MIN = std::numeric_limits<float>::min();

BoundingBox::BoundingBox(Vector3 c, float x, float y, float z, std::vector<float> v, std::vector<unsigned int> i)
	:center(c),
	radiusX(x),
	radiusY(y),
	radiusZ(z),
	vbo(&v[0], v.size() * sizeof(float)),
	layout(VertexBufferLayout()),
	vao(VertexArray()),
	ibo(&i[0], i.size() * sizeof(unsigned int))
{
	layout.PushAndNormalize<float>(3);
	vao.AddBuffer(vbo, layout);
}

BoundingBox::BoundingBox()
	:center(Vector3()), radiusX(DOUBLE_MIN), radiusY(DOUBLE_MIN), radiusZ(DOUBLE_MIN)
{}

void BoundingBox::Draw(Shader& shader) const
{
	Renderer::DrawWireFrame(vao, ibo.GetCount(), ibo, shader);
}

bool BoundingBox::HasCollided(const BoundingBox& rhs) const
{
	if (abs(center.x - rhs.center.x) > radiusX + rhs.radiusX) return false;
	if (abs(center.y - rhs.center.y) > radiusY + rhs.radiusY) return false;
	if (abs(center.z - rhs.center.z) > radiusZ + rhs.radiusZ) return false;
	return true;
}

void BoundingBox::UpdateCenter(float x, float y, float z)
{
	center.x += x;
	center.y += y;
	center.z += z;
}

BoundingBoxPrimitives::BoundingBoxPrimitives()
	:minX(std::numeric_limits<float>::max()),
	maxX(std::numeric_limits<float>::min()),
	minY(std::numeric_limits<float>::max()),
	maxY(std::numeric_limits<float>::min()),
	minZ(std::numeric_limits<float>::max()),
	maxZ(std::numeric_limits<float>::min())
{}

void BoundingBoxPrimitives::Print() const
{
	std::cout << "maxX: " << maxX << std::endl;
	std::cout << "minX: " << minX << std::endl;
	std::cout << "maxY: " << maxY << std::endl;
	std::cout << "minY: " << minY << std::endl;
	std::cout << "maxZ: " << maxZ << std::endl;
	std::cout << "minZ: " << minZ << std::endl;
}