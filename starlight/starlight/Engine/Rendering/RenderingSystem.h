#pragma once
#include "System.h"
#include "TransformComponent.h"
#include "RenderableComponent.h"
#include "ShaderComponent.h"

class RenderingSystem : public System<RenderableComponent, TransformComponent, ShaderComponent>
{
	using BaseType = System<RenderableComponent, TransformComponent, ShaderComponent>;
public:
	RenderingSystem()
		: BaseType(nullptr) {}
	RenderingSystem(Engine* pEntityEngine)
		: BaseType(pEntityEngine) {}

	virtual void Update(float deltaTime) override;

	static void Draw(RenderableComponent* rc, TransformComponent* tc, ShaderComponent* sh);

	static void TransferData(RenderableComponent* src, RenderableComponent* dst);

	static void Clear();
};