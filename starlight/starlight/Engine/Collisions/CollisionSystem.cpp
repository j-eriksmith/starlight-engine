#include "CollisionSystem.h"
#include "MemMgr.h"

CollisionComponentPtr CollisionSystem::GetCollisionComponent(RenderableComponentPtr component)
{
	BoundingBoxPrimitives& bb = component->bb;
	float midX = (bb.maxX - bb.minX) / 2;
	float midY = (bb.maxY - bb.minY) / 2;
	float midZ = (bb.maxZ - bb.minZ) / 2;
	Vector3 centerCoords(bb.minX + midX, bb.minY + midY, bb.minZ + midZ);
	std::vector<float> vertexData(CreateVertexData(bb));
	std::vector<unsigned int> indexData(CreateIndexData());
	return CollisionComponentPtr(new (MemMgr::Alloc(sizeof CollisionComponent, MemMgr::AllocatorType::LevelData))
		CollisionComponent(centerCoords, midX, midY, midZ, vertexData, indexData));
	// @TODO: store vertex array data for bounding box in the newly constructed bounding box object. 
	// Will need to create a vertex buffer layout object and pass in the vertex data.
}

std::vector<float> CollisionSystem::CreateVertexData(const BoundingBoxPrimitives& bb)
{
	return std::vector<float> {
		bb.minX, bb.maxY, bb.minZ, // top back left 0
			bb.minX, bb.maxY, bb.maxZ, // top front left 1
			bb.minX, bb.minY, bb.maxZ, // bottom front left 2
			bb.minX, bb.minY, bb.minZ, // bottom back left 3
			bb.maxX, bb.maxY, bb.minZ, // top back right 4
			bb.maxX, bb.maxY, bb.maxZ, // top front right 5
			bb.maxX, bb.minY, bb.maxZ, // bottom front right 6
			bb.maxX, bb.minY, bb.minZ // bottom back right 7
	};
}

std::vector<unsigned int> CollisionSystem::CreateIndexData()
{
	return std::vector<unsigned int>{
		// Left Face
		0, 2, 3,
			2, 0, 1,
			// Front Face
			1, 6, 2,
			6, 1, 5,
			// Right Face
			5, 7, 6,
			7, 5, 4,
			// Back Face
			4, 3, 7,
			3, 4, 0,
			// Top Face
			0, 5, 1,
			5, 0, 4,
			// Bottom Face
			2, 7, 3,
			7, 2, 6
	};
}

void CollisionSystem::Update(float deltaTime)
{
	Log("CollisionSystem::Update");
	for (auto& ComponentTuple : Components)
	{
		CollisionComponentPtr collisionComponent = CollisionComponentPtr(std::get<CollisionComponent*>(ComponentTuple));
		TransformComponentPtr transformComponent = TransformComponentPtr(std::get<TransformComponent*>(ComponentTuple));
		UpdateCenterPoint(collisionComponent, transformComponent);
	}
}

void CollisionSystem::UpdateCenterPoint(CollisionComponentPtr cc, TransformComponentPtr tc)
{
	glm::vec4 c = tc->Data.GetGlmMat4() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	cc->center.x = c.x;
	cc->center.y = c.y;
	cc->center.z = c.z;
}

void CollisionSystem::TransferData(CollisionComponent* src, CollisionComponent* dst)
{
	dst->center = src->center;
	dst->radiusX = src->radiusX;
	dst->radiusY = src->radiusY;
	dst->radiusZ = src->radiusZ;
	dst->shouldRender = src->shouldRender;
	dst->vbo.reset(src->vbo.get());
	dst->vao.reset(src->vao.get());
	dst->ibo.reset(src->ibo.get());
	dst->layout.reset(src->layout.get());
}