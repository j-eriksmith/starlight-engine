#pragma once
#include "System.h"
#include "TransformComponent.h"
#include "RenderableComponent.h"
#include "ShaderComponent.h"
#include "CollisionComponent.h"

class RenderingSystem : public System<RenderableComponent, TransformComponent, ShaderComponent, CollisionComponent>
{
	using BaseType = System<RenderableComponent, TransformComponent, ShaderComponent, CollisionComponent>;
public:
	RenderingSystem()
		: BaseType(nullptr) {}
	RenderingSystem(Engine* pEntityEngine)
		: BaseType(pEntityEngine) {}

	virtual void Update(float deltaTime) override;

	static void Draw(RenderableComponent* rc, TransformComponent* tc, ShaderComponent* sh, CollisionComponent* cc);

	static void DrawBoundingBox(CollisionComponent* cc, ShaderComponent* sh);

	static void TransferData(RenderableComponent* src, RenderableComponent* dst);

	static void Clear();
};