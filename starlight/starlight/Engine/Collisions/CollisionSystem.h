#pragma once
#include "System.h"
#include "CollisionComponent.h"
#include "RenderableComponent.h"
#include "TransformComponent.h"
#include <string>

using CollisionComponentPtr = std::shared_ptr<CollisionComponent>;
using CollidableType = CollisionComponent::CollidableType;
using ComponentTuple = std::tuple< CollisionComponent*, TransformComponent*, RenderableComponent* >;

class CollisionSystem : public System<CollisionComponent, TransformComponent, RenderableComponent>
{
	using BaseType = System<CollisionComponent, TransformComponent, RenderableComponent>;
public:
	CollisionSystem()
		: BaseType(nullptr) {}
	CollisionSystem(Engine* pEntityEngine)
		: BaseType(pEntityEngine) {}

	virtual void Update(float deltaTime) override;

	void UpdateCenterPoint(CollisionComponent* cc, TransformComponent* tc);

	static CollisionComponentPtr GetCollisionComponent(RenderableComponentPtr component);

	static std::vector<float> CreateVertexData(const BoundingBoxPrimitives& bb);

	static std::vector<unsigned int> CreateIndexData();

	void ResolveLhsCollidableType(ComponentTuple* lhs, ComponentTuple* rhs);

	template<CollidableType T>
	void CallResolveCollision(ComponentTuple* lhs, ComponentTuple* rhs);

	template<>
	void CallResolveCollision<CollidableType::Friendly>(ComponentTuple* lhs, ComponentTuple* rhs);

	template<>
	void CallResolveCollision<CollidableType::Enemy>(ComponentTuple* lhs, ComponentTuple* rhs);

	template<>
	void CallResolveCollision<CollidableType::Projectile>(ComponentTuple* lhs, ComponentTuple* rhs);

	template<>
	void CallResolveCollision<CollidableType::Structure>(ComponentTuple* lhs, ComponentTuple* rhs);

	template<CollidableType l,
			 CollidableType r>
	static void ResolveCollision(ComponentTuple* lhs, ComponentTuple* rhs);

	template<>
	static void ResolveCollision<CollidableType::Friendly,
		CollidableType::Friendly>(ComponentTuple* lhs, ComponentTuple* rhs);
	template<>
	static void ResolveCollision<CollidableType::Friendly,
		CollidableType::Projectile>(ComponentTuple* lhs, ComponentTuple* rhs);

	template<>
	static void ResolveCollision<CollidableType::Projectile,
		CollidableType::Friendly>(ComponentTuple* lhs, ComponentTuple* rhs);
	template<>
	static void ResolveCollision<CollidableType::Friendly,
		CollidableType::Enemy>(ComponentTuple* lhs, ComponentTuple* rhs);

	template<>
	static void ResolveCollision<CollidableType::Enemy,
		CollidableType::Friendly>(ComponentTuple* lhs, ComponentTuple* rhs);
	template<>
	static void ResolveCollision<CollidableType::Friendly,
		CollidableType::Structure>(ComponentTuple* lhs, ComponentTuple* rhs);

	template<>
	static void ResolveCollision<CollidableType::Structure,
		CollidableType::Friendly>(ComponentTuple* lhs, ComponentTuple* rhs);

	template<>
	static void ResolveCollision<CollidableType::Projectile,
		CollidableType::Structure>(ComponentTuple* lhs, ComponentTuple* rhs);
	template<>
	static void ResolveCollision<CollidableType::Structure,
		CollidableType::Projectile>(ComponentTuple* lhs, ComponentTuple* rhs);

	static std::string GetCollisionTypeString(CollisionComponent* c);
	static void TransferData(CollisionComponent* src, CollisionComponent* dst);
};