#pragma once
#include "System.h"
#include "CollisionComponent.h"
#include "RenderableComponent.h"
#include "TransformComponent.h"
#include <string>

using CollisionComponentPtr = std::shared_ptr<CollisionComponent>;
using CollidableType = CollisionComponent::CollidableType;

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

	void CollisionSystem::CallResolveCollision(CollisionComponentPtr lhs, CollisionComponentPtr rhs);

	template<CollidableType lhs,
			 CollidableType rhs>
	static void ResolveCollision(CollisionComponentPtr lhs, CollisionComponentPtr rhs);

	template<>
	static void ResolveCollision<CollidableType::Friendly,
		CollidableType::Friendly>(CollisionComponentPtr lhs, CollisionComponentPtr rhs);
	template<>
	static void ResolveCollision<CollidableType::Friendly,
		CollidableType::Projectile>(CollisionComponentPtr lhs, CollisionComponentPtr rhs);

	template<>
	static void ResolveCollision<CollidableType::Projectile,
		CollidableType::Friendly>(CollisionComponentPtr lhs, CollisionComponentPtr rhs);
	template<>
	static void ResolveCollision<CollidableType::Friendly,
		CollidableType::Enemy>(CollisionComponentPtr lhs, CollisionComponentPtr rhs);

	template<>
	static void ResolveCollision<CollidableType::Enemy,
		CollidableType::Friendly>(CollisionComponentPtr lhs, CollisionComponentPtr rhs);
	template<>
	static void ResolveCollision<CollidableType::Friendly,
		CollidableType::Structure>(CollisionComponentPtr lhs, CollisionComponentPtr rhs);

	template<>
	static void ResolveCollision<CollidableType::Structure,
		CollidableType::Friendly>(CollisionComponentPtr lhs, CollisionComponentPtr rhs);

	static std::string GetCollisionTypeString(CollisionComponentPtr c);
	static void TransferData(CollisionComponent* src, CollisionComponent* dst);
};