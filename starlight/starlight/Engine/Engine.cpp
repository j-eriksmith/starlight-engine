#include "Engine.h"
#include "MovementSystem.h"
#include "DamageInRangeSystem.h"
#include "Rendering/CollisionComponent.h"
#include "Rendering/RenderableComponent.h"
#include "MemMgr.h"
#include "Debug.h"
#include "Input/Input.h"

void Engine::Update(float deltaTime)
{
	Input::Update();
	for (size_t i = 0; i < AllSystems.size(); i++)
	{
		// tell each system to update
		AllSystems[i]->Update(deltaTime);
	}
}

template <typename SystemType>
void Engine::AddSystem()
{
	uint8_t* SystemMemory = MemMgr::Alloc(sizeof(SystemType), MemMgr::AllocatorType::PoolData);
	AllSystems.push_back(new (SystemMemory) SystemType(this));
}

Entity* Engine::CreateEntity()
{
	// Todo(jake): automatically pick a valid ID so programmers don't have to keep track of them
	// -- this mechanism needs to pick lower IDs once entities with lower IDs are destroyed
	// -- query the engine or something that has used entity IDs mapped

	static EntityID NextUnusedID = 0;
	Entity* CreatedEntity = new (MemMgr::Alloc(sizeof(Entity), MemMgr::AllocatorType::PoolData)) Entity(this, NextUnusedID);
	++NextUnusedID;

	// Notify systems
	for (size_t i = 0; i < AllSystems.size(); ++i)
	{
		AllSystems[i]->OnEntityCreated(*CreatedEntity);
	}

	// Update our bookkeeping of all entities
	AllEntities.push_back(CreatedEntity);

	return CreatedEntity;
}

void Engine::DestroyEntity(EntityID entityID)
{
	// Ensure that we actually have this entity and not a bogus ID
	// ..in our implementation, it's a linear search
	Entity* EntityToDestroy = nullptr;
	for (auto EntityCursor = AllEntities.begin(); EntityCursor != AllEntities.end(); ++EntityCursor)
	{
		if ((*EntityCursor)->GetID() == entityID)
		{
			EntityToDestroy = *EntityCursor;
			AllEntities.erase(EntityCursor);
			break;
		}
	}

	if (EntityToDestroy == nullptr)
	{
		Log(std::string("Error: Entity ") + std::to_string(entityID) + std::string(" not found in Engine."));
		return;
	}

	// Todo(jake): Deallocate entity once the deallocation API doesn't use MemoryResource*

	// Notify systems
	for (size_t i = 0; i < AllSystems.size(); ++i)
	{
		AllSystems[i]->OnEntityDestroyed(entityID);
	}
}

Engine::~Engine()
{
	// Todo(jake): Waiting for deallocation API to be cleaned up before commiting to skeleton code.
	// I will deallocate the elements of Engine's containers here directly instead of relying on type destructors. 
}

void Engine::AddAllSystems()
{
	// All ystems need to be added here to be updated in the game loop. Their order here is their update order every frame. 
	AddSystem<MovementSystem>();
	AddSystem<DamageInRangeSystem>();
}

template <class CompType>
CompType* Engine::AllocateComponent(Entity& entityToAllocateFor)
{
	uint8_t* MemoryForComponent = MemMgr::Alloc(sizeof(CompType), MemMgr::AllocatorType::PoolData);

	// Has this component been allocated?	
	// If so, put it at the back of that type's vector
	if (CompType::EngineMemoryID < std::numeric_limits<unsigned int>::max())
	{
		AllComponents[CompType::EngineMemoryID].push_back(new (MemoryForComponent) CompType());
	}
	else
	{
		// If not, put it at the back of this vector with vector::size()
		AllComponents.push_back(std::vector<Component*>()); // allocate another row vector for this component type
		AllComponents[AllComponents.size() - 1].push_back(new (MemoryForComponent) CompType()); // push this component into its new row vector
		CompType::EngineMemoryID = AllComponents.size() - 1;
	}

	unsigned int IndexOfNewComponent = AllComponents[CompType::EngineMemoryID].size() - 1;
	CompType* AllocatedComponent = static_cast<CompType*>(AllComponents[CompType::EngineMemoryID][IndexOfNewComponent]);
	entityToAllocateFor.GetComponents().emplace(CompType::UniqueID, AllocatedComponent);

	// Notify systems in case this entity is now interesting to them
	for (size_t i = 0; i < AllSystems.size(); ++i)
	{
		AllSystems[i]->OnEntityCreated(entityToAllocateFor);
	}

	return AllocatedComponent;
}

void Engine::InitTest()
{
	Entity* e1 = CreateEntity();
	Entity* e2 = CreateEntity();
	Entity* e3 = CreateEntity();

	/* Transform Testing */
	e1->AddComponent<TransformComponent>();
	TransformComponent* e2_transform = e2->AddComponent<TransformComponent>(); 
	*e2_transform = TransformComponent(Vector3(.92303f, .32139f, -.21147f),
		Vector3(-.21147f, .88302f, .41899f),
		Vector3(.32139f, -.34202f, .88302f),
		Vector3(10.f, 10.f, 10.f),
		e1->GetComponent<TransformComponent>());
	TransformComponent* e3_transform = e3->AddComponent<TransformComponent>();
	e3_transform->Data = e3->GetComponent<TransformComponent>()->Data.Translate(Vector3(10, 10, 10));

	/* Health Component Testing */
	HealthComponent* e1_health = e1->AddComponent<HealthComponent>();
	e1_health->CurrentHealth = 100.0f;
	e1_health->MaxHealth = e1_health->CurrentHealth;

	HealthComponent* e2_health = e2->AddComponent<HealthComponent>();
	e2_health->CurrentHealth = 100.0f;
	e2_health->MaxHealth = e2_health->CurrentHealth;
}

// initialization is here because Components don't have a .cpp file
unsigned int TransformComponent::EngineMemoryID = UINT32_MAX; 
unsigned int HealthComponent::EngineMemoryID = UINT32_MAX; 
unsigned int CollisionComponent::EngineMemoryID = UINT32_MAX;
unsigned int RenderableComponent::EngineMemoryID = UINT32_MAX;
unsigned int ShaderComponent::EngineMemoryID = UINT32_MAX;