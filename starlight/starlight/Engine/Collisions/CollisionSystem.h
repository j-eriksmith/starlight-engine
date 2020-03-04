#pragma once
#include "System.h"
#include "CollisionComponent.h"
#include "RenderableComponent.h"
#include "TransformComponent.h"

using CollisionComponentPtr = std::shared_ptr<CollisionComponent>;
class CollisionSystem : public System<CollisionComponent, TransformComponent>
{
	using BaseType = System<CollisionComponent, TransformComponent>;
public:
	CollisionSystem()
		: BaseType(nullptr) {}
	CollisionSystem(Engine* pEntityEngine)
		: BaseType(pEntityEngine) {}

	virtual void Update(float deltaTime) override;

	void UpdateCenterPoint(CollisionComponentPtr cc, TransformComponentPtr tc);

	static CollisionComponentPtr GetCollisionComponent(RenderableComponentPtr component);

	static std::vector<float> CreateVertexData(const BoundingBoxPrimitives& bb);

	static std::vector<unsigned int> CreateIndexData();

	static void TransferData(CollisionComponent* src, CollisionComponent* dst);
};