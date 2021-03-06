#pragma once
#include "Component.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "vec3.h"
#include "String/StringHash.h"

class CollisionComponent : public Component
{
public:
	static enum class CollidableType {
		Friendly = 0,
		Enemy = 1,
		Projectile = 2,
		Structure = 3,
		Dart = 4
	};

	explicit CollisionComponent()
		:Component(UniqueID),
		center(Vector3(0.0f, 0.0f, 0.0f)),
		origin(Vector3(0.f, 0.f, 0.f)),
		radiusX(0.0f),
		radiusY(0.0f),
		radiusZ(0.0f),
		shouldRender(false),
		pendingDestroyOwningEntity(false),
		vbo(nullptr),
		layout(nullptr),
		vao(nullptr),
		ibo(nullptr),
		collidableType(CollidableType::Friendly)
	{}
	
	CollisionComponent(
		Vector3 c,
		Vector3 o,
		float x, 
		float y, 
		float z, 
		std::vector<float> v, 
		std::vector<unsigned int> i, 
		bool render = true,
		CollidableType type = CollidableType::Friendly
	    )
		:Component(UniqueID),
		center(c),
		origin(o),
		radiusX(x),
		radiusY(y),
		radiusZ(z),
		shouldRender(true),
		pendingDestroyOwningEntity(false),
		vbo(new VertexBuffer(&v[0], v.size() * sizeof(float))),
		layout(new VertexBufferLayout()),
		vao(new VertexArray()),
		ibo(new IndexBuffer(&i[0], i.size() * sizeof(unsigned int))),
		collidableType(type)
	{
		layout->PushAndNormalize<float>(3);
		vao->AddBuffer(*vbo, *layout);
	}

	static constexpr ComponentID UniqueID = StringIdHash("CollisionComponent");
	static unsigned int EngineMemoryID;

	Vector3 center;
	Vector3 origin;
	float radiusX;
	float radiusY;
	float radiusZ;
	bool shouldRender;
	bool pendingDestroyOwningEntity;
	std::shared_ptr<VertexBuffer> vbo;
	std::shared_ptr<VertexBufferLayout> layout;
	std::shared_ptr<VertexArray> vao;
	std::shared_ptr<IndexBuffer> ibo;
	CollidableType collidableType;
};