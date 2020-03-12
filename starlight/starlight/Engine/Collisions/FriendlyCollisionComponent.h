#pragma once
#include "CollisionComponent.h"

class ProjectileCollisionComponent : public CollisionComponent
{
	explicit ProjectileCollisionComponent()
		:CollisionComponent()
	{}

	ProjectileCollisionComponent(
		Vector3 c,
		float x,
		float y,
		float z,
		std::vector<float> v,
		std::vector<unsigned int> i,
		bool render = true
	)
		:CollisionComponent(c, x, y, z, v, i, render)
	{}
};