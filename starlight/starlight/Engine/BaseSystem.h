#pragma once

class Engine; // Since this file is included by Engine.h

class BaseSystem
{
protected: 
	Engine* EntityEngine;
	bool Enabled;
	friend class Engine;

public:
	BaseSystem(Engine* pEntityEngine)
		: EntityEngine(pEntityEngine),
			Enabled(true) {}

	virtual ~BaseSystem() = default;
	virtual void OnEntityCreated(Entity& entity) = 0;
	virtual void OnEntityDestroyed(EntityID entity) = 0;
	virtual void OnComponentRemoved(EntityID entity, ComponentID compID) = 0;
	virtual void Update(float deltaTime) = 0;
	void SetEngine(Engine* pEntityEngine) { EntityEngine = pEntityEngine; }
};
