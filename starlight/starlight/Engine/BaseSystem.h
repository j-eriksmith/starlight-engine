#pragma once

class Engine; // Since this file is included by Engine.h

class BaseSystem
{
protected: 
	Engine* EntityEngine;

public:
	BaseSystem(Engine* pEntityEngine)
		: EntityEngine(pEntityEngine) {}

	virtual ~BaseSystem() = default;
	virtual void OnEntityCreated(const Entity& entity) = 0;
	virtual void OnEntityDestroyed(EntityID entity) = 0;
	virtual void Update(float deltaTime) = 0;
	void SetEngine(Engine* pEntityEngine) { EntityEngine = pEntityEngine; }
};
