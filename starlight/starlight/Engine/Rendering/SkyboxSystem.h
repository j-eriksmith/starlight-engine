#pragma once
#include "CubemapComponent.h"
#include "ShaderComponent.h"
#include "TransformComponent.h"
#include "System.h"

class SkyboxSystem : public System<CubemapComponent, ShaderComponent>
{
public:
	using BaseType = System<CubemapComponent, ShaderComponent>;

	SkyboxSystem()
		: BaseType(nullptr) {}
	SkyboxSystem(Engine* pEntityEngine)
		: BaseType(pEntityEngine) {}

	virtual void Update(float deltaTime) override;
	void Draw(CubemapComponent* cubemap, ShaderComponent* shader) const;

	// Component initialization functions
	static void LoadTextures(const std::array<std::string, 6>& texturesToLoad, CubemapComponent& cubemapComp);
};
