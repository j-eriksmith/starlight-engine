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
	for (auto& CompTuple : Components)
	{
		CollisionComponent* collisionComponent = std::get<CollisionComponent*>(CompTuple);
		TransformComponent* transformComponent = std::get<TransformComponent*>(CompTuple);
		UpdateCenterPoint(collisionComponent, transformComponent);
	}
	for (int i = 0; i < Components.size(); ++i)
	{
		CollisionComponentPtr lhs = CollisionComponentPtr(std::get<CollisionComponent*>(Components[i]));
		for (int j = i; j < Components.size(); ++j)
		{
			CollisionComponentPtr rhs = CollisionComponentPtr(std::get<CollisionComponent*>(Components[j]));
			//CallResolveCollision<CollidableType::Friendly, CollidableType::Friendly>(lhs, rhs);
		}
	}
}

void CollisionSystem::UpdateCenterPoint(CollisionComponent* cc, TransformComponent* tc)
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


void CollisionSystem::ResolveLhsCollidableType( ComponentTuple* lhs, ComponentTuple* rhs)
{
	CollisionComponent* colComp = std::get<CollisionComponent*>(*lhs);
	switch (colComp->collidableType)
	{
		case CollidableType::Friendly:
		{
			CallResolveCollision<CollidableType::Friendly>(lhs, rhs);
			break;
		}
		case CollidableType::Enemy:
		{
			CallResolveCollision<CollidableType::Enemy>(lhs, rhs);
			break;
		}
		case CollidableType::Projectile:
		{
			CallResolveCollision<CollidableType::Projectile>(lhs, rhs);
			break;
		}
		case CollidableType::Structure:
		{
			CallResolveCollision<CollidableType::Structure>(lhs, rhs);
			break;
		}
	}
}

template <CollidableType T>
void CollisionSystem::CallResolveCollision(ComponentTuple* lhs, ComponentTuple* rhs) {
	CollisionComponent* colComp = std::get<CollisionComponent*>(*lhs);
	Log("CollisionSystem::CallResolveCollision -- type " << GetCollisionTypeString(colComp) << " not supported.");
}

template <>
void CollisionSystem::CallResolveCollision<CollidableType::Friendly>(ComponentTuple* lhs, ComponentTuple* rhs) {
	CollisionComponent* colComp = std::get<CollisionComponent*>(*rhs);
	switch (colComp->collidableType)
	{
		case CollidableType::Friendly:
		{
			ResolveCollision<CollidableType::Friendly, CollidableType::Friendly>(lhs, rhs);
			break;
		}
		case CollidableType::Enemy:
		{
			ResolveCollision<CollidableType::Friendly, CollidableType::Enemy>(lhs, rhs);
			break;
		}
		case CollidableType::Projectile:
		{
			ResolveCollision<CollidableType::Friendly, CollidableType::Projectile>(lhs, rhs);
			break;
		}
		case CollidableType::Structure:
		{
			ResolveCollision<CollidableType::Friendly, CollidableType::Structure>(lhs, rhs);
			break;
		}
		default:
		{
			ResolveCollision<CollidableType::Friendly, CollidableType::Friendly>(lhs, rhs);
			Log("CollisionSystem::CallResolveCollision<Friendly> -- Resorting to default collision resolution");
		}
	}
}

template <>
void CollisionSystem::CallResolveCollision<CollidableType::Enemy>(ComponentTuple* lhs, ComponentTuple* rhs) {
	CollisionComponent* colComp = std::get<CollisionComponent*>(*rhs);
	switch (colComp->collidableType)
	{
	case CollidableType::Friendly:
	{
		ResolveCollision<CollidableType::Enemy, CollidableType::Friendly>(lhs, rhs);
		break;
	}
	case CollidableType::Enemy:
	{
		ResolveCollision<CollidableType::Enemy, CollidableType::Enemy>(lhs, rhs);
		break;
	}
	case CollidableType::Projectile:
	{
		ResolveCollision<CollidableType::Enemy, CollidableType::Projectile>(lhs, rhs);
		break;
	}
	case CollidableType::Structure:
	{
		ResolveCollision<CollidableType::Enemy, CollidableType::Structure>(lhs, rhs);
		break;
	}
	default:
	{
		ResolveCollision<CollidableType::Enemy, CollidableType::Enemy>(lhs, rhs);
		Log("CollisionSystem::CallResolveCollision<Enemy> -- Resorting to default collision resolution");
	}

	}
}

template <>
void CollisionSystem::CallResolveCollision<CollidableType::Projectile>(ComponentTuple* lhs, ComponentTuple* rhs) {
	CollisionComponent* colComp = std::get<CollisionComponent*>(*rhs);
	switch (colComp->collidableType)
	{
	case CollidableType::Friendly:
	{
		ResolveCollision<CollidableType::Projectile, CollidableType::Friendly>(lhs, rhs);
		break;
	}
	case CollidableType::Enemy:
	{
		ResolveCollision<CollidableType::Projectile, CollidableType::Enemy>(lhs, rhs);
		break;
	}
	case CollidableType::Projectile:
	{
		ResolveCollision<CollidableType::Projectile, CollidableType::Projectile>(lhs, rhs);
		break;
	}
	case CollidableType::Structure:
	{
		ResolveCollision<CollidableType::Projectile, CollidableType::Structure>(lhs, rhs);
		break;
	}
	default:
	{
		ResolveCollision<CollidableType::Projectile, CollidableType::Projectile>(lhs, rhs);
		Log("CollisionSystem::CallResolveCollision<Projectile> -- Resorting to default collision resolution");
	}

	}
}

template <>
void CollisionSystem::CallResolveCollision<CollidableType::Structure>(ComponentTuple* lhs, ComponentTuple* rhs) {
	CollisionComponent* colComp = std::get<CollisionComponent*>(*rhs);
	switch (colComp->collidableType)
	{
	case CollidableType::Friendly:
	{
		ResolveCollision<CollidableType::Structure, CollidableType::Friendly>(lhs, rhs);
		break;
	}
	case CollidableType::Enemy:
	{
		ResolveCollision<CollidableType::Structure, CollidableType::Enemy>(lhs, rhs);
		break;
	}
	case CollidableType::Projectile:
	{
		ResolveCollision<CollidableType::Structure, CollidableType::Projectile>(lhs, rhs);
		break;
	}
	case CollidableType::Structure:
	{
		ResolveCollision<CollidableType::Structure, CollidableType::Structure>(lhs, rhs);
		break;
	}
	default:
	{
		ResolveCollision<CollidableType::Structure, CollidableType::Structure>(lhs, rhs);
		Log("CollisionSystem::CallResolveCollision<Structure> -- Resorting to default collision resolution");
	}
	}
}

template<CollidableType l,
	CollidableType r>
	void CollisionSystem::ResolveCollision(ComponentTuple* lhs, ComponentTuple* rhs)
{
	CollisionComponent* lhsColComp = std::get<CollisionComponent*>(*rhs);
	CollisionComponent* rhsColComp = std::get<CollisionComponent*>(*rhs);
	Log("CollisionSystem::ResolveCollision -- Resolution between " << GetCollisionTypeString(lhsColComp) << " and " << GetCollisionTypeString(rhsColComp) << " is not defined.");
}

template<>
void CollisionSystem::ResolveCollision<CollidableType::Friendly,
									   CollidableType::Friendly>
	(
		ComponentTuple* lhs,
		ComponentTuple* rhs
	)
{
	CollisionComponent* colComp = std::get<CollisionComponent*>(*rhs);
	Log("CollisionSystem::ResolveFriendlyCollision -- Resolved Collision between Friendly and " << GetCollisionTypeString(colComp) << " by doing nothing.");
}

template<>
void CollisionSystem::ResolveCollision<CollidableType::Friendly,
	CollidableType::Enemy>
	(
		ComponentTuple* lhs,
		ComponentTuple* rhs
		)
{
	CollisionComponent* colComp = std::get<CollisionComponent*>(*rhs);
	RenderableComponent* rhsRC = std::get<RenderableComponent*>(*rhs);
	rhsRC->shouldRender = false;
}

template<>
void CollisionSystem::ResolveCollision<CollidableType::Enemy,
	CollidableType::Friendly>
	(
		ComponentTuple* lhs,
		ComponentTuple* rhs
		)
{
	CollisionComponent* colComp = std::get<CollisionComponent*>(*rhs);
	Log("CollisionSystem::ResolveFriendlyCollision -- Resolved Collision between Friendly and " << GetCollisionTypeString(colComp) << " by doing nothing.");
}

template<>
void CollisionSystem::ResolveCollision<CollidableType::Friendly,
	CollidableType::Projectile>
	(
		ComponentTuple* lhs,
		ComponentTuple* rhs
		)
{
	CollisionComponent* colComp = std::get<CollisionComponent*>(*rhs);
	Log("CollisionSystem::ResolveFriendlyCollision -- Resolved Collision between Friendly and " << GetCollisionTypeString(colComp) << " by doing nothing.");
}

template<>
void CollisionSystem::ResolveCollision<CollidableType::Projectile,
	CollidableType::Friendly>
	(
		ComponentTuple* lhs,
		ComponentTuple* rhs
		)
{
	CollisionComponent* colComp = std::get<CollisionComponent*>(*rhs);
	Log("CollisionSystem::ResolveFriendlyCollision -- Resolved Collision between Friendly and " << GetCollisionTypeString(colComp) << " by doing nothing.");
}

template<>
void CollisionSystem::ResolveCollision<CollidableType::Friendly,
	CollidableType::Structure>
	(
		ComponentTuple* lhs,
		ComponentTuple* rhs
		)
{
	CollisionComponent* colComp = std::get<CollisionComponent*>(*rhs);
	Log("CollisionSystem::ResolveFriendlyCollision -- Resolved Collision between Friendly and " << GetCollisionTypeString(colComp) << " by doing nothing.");
}

template<>
void CollisionSystem::ResolveCollision<CollidableType::Structure,
	CollidableType::Friendly>
	(
		ComponentTuple* lhs,
		ComponentTuple* rhs
		)
{
	CollisionComponent* colComp = std::get<CollisionComponent*>(*rhs);
	Log("CollisionSystem::ResolveFriendlyCollision -- Resolved Collision between Friendly and " << GetCollisionTypeString(colComp) << " by doing nothing.");
}

template<>
static void CollisionSystem::ResolveCollision<CollidableType::Projectile,
	CollidableType::Structure>(ComponentTuple* lhs, ComponentTuple* rhs)
{
	CollisionComponent* colComp = std::get<CollisionComponent*>(*rhs);
	Log("CollisionSystem::ResolveFriendlyCollision -- Resolved Collision between Friendly and " << GetCollisionTypeString(colComp) << " by doing nothing.");
}

template<>
static void CollisionSystem::ResolveCollision<CollidableType::Structure,
	CollidableType::Projectile>(ComponentTuple* lhs, ComponentTuple* rhs)
{
	CollisionComponent* colComp = std::get<CollisionComponent*>(*rhs);
	Log("CollisionSystem::ResolveFriendlyCollision -- Resolved Collision between Friendly and " << GetCollisionTypeString(colComp) << " by doing nothing.");
}

std::string CollisionSystem::GetCollisionTypeString(CollisionComponent* c)
{
	using ColType = CollisionComponent::CollidableType;
	return c->collidableType == ColType::Friendly ? "Friendly" :
		c->collidableType == ColType::Enemy ? "Enemy" :
		c->collidableType == ColType::Projectile ? "Projectile" :
		c->collidableType == ColType::Structure ? "Structure" :
		"Unknown";
}