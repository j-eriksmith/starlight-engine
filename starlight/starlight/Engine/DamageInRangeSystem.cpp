#include "DamageInRangeSystem.h"

void DamageInRangeSystem::Update(float deltaTime)
{
	for (size_t obj1 = 0; obj1 < Components.size() - 1; ++obj1)
	{
		for (size_t obj2 = obj1 + 1; obj2 < Components.size(); ++obj2)
		{
			auto& CompTuple1 = Components[obj1];
			auto& CompTuple2 = Components[obj2];
			Transform& TransformData1 = std::get<TransformComponent*>(CompTuple1)->Data;
			HealthComponent* HealthComp1 = std::get<HealthComponent*>(CompTuple1);
			Transform& TransformData2 = std::get<TransformComponent*>(CompTuple2)->Data;
			HealthComponent* HealthComp2 = std::get<HealthComponent*>(CompTuple2);

			if ((TransformData1.Origin - TransformData2.Origin).LengthSquared() <= 1000.0f)
			{
				CheckAndDealDamage(HealthComp1);
				CheckAndDealDamage(HealthComp2);
			}
		}
	}
}

void DamageInRangeSystem::CheckAndDealDamage(HealthComponent* HealthComp)
{
	HealthComp->CurrentHealth -= 5.0f;
	if (HealthComp->CurrentHealth <= 0.0f)
	{
		// They're dead
		HealthComp->CurrentHealth = 0.0f;
	}
}
