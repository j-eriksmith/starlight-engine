#pragma once
#include "System.h"
#include "TransformComponent.h"
#include "HealthComponent.h"

class DamageInRangeSystem : public System<TransformComponent, HealthComponent>
{
	using BaseType = System<TransformComponent, HealthComponent>;
public:
	DamageInRangeSystem()
		: BaseType(nullptr) {}
	DamageInRangeSystem(Engine* pEntityEngine)
		: BaseType(pEntityEngine) {}

	virtual void Update(float deltaTime) override;
protected:
	virtual void CheckAndDealDamage(HealthComponent* HealthComp);
};
