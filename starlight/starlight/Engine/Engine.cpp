#include "Engine.h"
#include "MovementSystem.h"
#include "MemMgr.h"

void Engine::Update( /* delta if used variable game loop */ )
{
	for (int i = 0; i < NUM_SYSTEMS; i++)
	{
		// tell each system to update
		Systems[i]->Update(0.02f);
	}
}

void Engine::AddSystem(BaseSystem* const system)
{
	Systems.push_back(system);
}

void Engine::AddEntity(Entity& entity)
{
	for (int i = 0; i < NUM_SYSTEMS; ++i)
	{
		Systems[i]->OnEntityCreated(entity);
	}
	AllEntities.push_back(std::move(entity));
}

void Engine::InitTest()
{
	// Add system
	// AddSystem(new MovementSystem(this));
	MemoryResource* ms_memory = MemMgr::Alloc(sizeof(MovementSystem), MemMgr::AllocatorType::PoolData);
	BaseSystem* ms = reinterpret_cast<MovementSystem*>(ms_memory->addr);
	*ms = MovementSystem(this);
	(*ms).Update(0.02f);
	
	// Add entity
	Entity e(0);
	Entity e1(1);
	Entity e2(2);
	e.AddComponent(TransformComponent::ID, new TransformComponent());
	e1.AddComponent(TransformComponent::ID, new TransformComponent(Vector3(.92303f, .32139f, -.21147f),
		Vector3(-.21147f, .88302f, .41899f),
		Vector3(.32139f, -.34202f, .88302f),
		Vector3(10.f, 10.f, 10.f),
		e.GetComponent<TransformComponent>()));
	e2.AddComponent(TransformComponent::ID, new TransformComponent(e1.GetComponent<TransformComponent>()));
	e2.GetComponent<TransformComponent>()->Data = e2.GetComponent<TransformComponent>()->Data.Translate(Vector3(10, 10, 10));
	AddEntity(e);
	AddEntity(e1);
	AddEntity(e2);
}
