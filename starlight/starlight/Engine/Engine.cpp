#include "Engine.h"
#include "MovementSystem.h"
#include "DamageInRangeSystem.h"
#include "CollisionComponent.h"
#include "RenderableComponent.h"
#include "TransformComponent.h"
#include "ShaderComponent.h"
#include "Game/PlayerComponent.h"
#include "RenderingSystem.h"
#include "ShaderSystem.h"
#include "ModelLoadingSystem.h"
#include "SkyboxSystem.h"
#include "Game/ThrowDartSystem.h"
#include "MemMgr.h"
#include "Debug.h"
#include "Input/Input.h"

void Engine::Update(float deltaTime)
{
	Input::Update();
	for (size_t i = 0; i < AllSystems.size(); i++)
	{
		// tell each system to update if enabled
		if (AllSystems[i]->Enabled)
		{
			Log("Engine::Update -- Updating...");
			AllSystems[i]->Update(deltaTime);
		}
	}
}

template <typename SystemType>
void Engine::AddSystem()
{
	uint8_t* SystemMemory = MemMgr::Alloc(sizeof(SystemType), MemMgr::AllocatorType::PoolData);
	AllSystems.push_back(new (SystemMemory) SystemType(this));
}

template <typename SystemType>
void Engine::EnableSystem()
{
	for (auto SystemCursor = AllSystems.begin(); SystemCursor != AllSystems.end(); ++SystemCursor)
	{
		SystemType* CastedSystemCursor = dynamic_cast<SystemType*>(*SystemCursor);
		if (CastedSystemCursor)
		{
			CastedSystemCursor->Enabled = true;
			return;
		}
	}
	Log("System to enable could not be found");
}

template <typename SystemType>
void Engine::DisableSystem()
{
	for (auto SystemCursor = AllSystems.begin(); SystemCursor != AllSystems.end(); ++SystemCursor)
	{
		SystemType* CastedSystemCursor = dynamic_cast<SystemType*>(*SystemCursor);
		if (CastedSystemCursor)
		{
			CastedSystemCursor->Enabled = false;
			return;
		}
	}
	Log("System to disable could not be found");
}

template <typename SystemType>
void Engine::RemoveSystem()
{
	for (auto SystemCursor = AllSystems.begin(); SystemCursor != AllSystems.end(); ++SystemCursor)
	{
		SystemType* CastedSystemCursor = dynamic_cast<SystemType*>(*SystemCursor);
		if (CastedSystemCursor)
		{
			// Erase system from our logging and deallocate
			AllSystems.erase(SystemCursor);
			MemMgr::Free(sizeof(*CastedSystemCursor), reinterpret_cast<uint8_t*>(CastedSystemCursor));
			return;
		}
	}
	Log("Error: System not found in engine on RemoveSystem()");
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

	// Deallocate all components associated with this entity
	for (auto& CompIDPair : EntityToDestroy->GetComponents())
	{
		unsigned int EngineMemoryID = CompMemoryIDMap[CompIDPair.first];
		unsigned int CompVectorIndex = CompIDPair.second->IndexInCompVector;

		// If not the backmost comp of this type... put backmost element of this compType into this CompVectorIndex
		if (CompVectorIndex + 1 < AllComponents[EngineMemoryID].size())
		{
			// ..and update the bookkeeping on that component
			AllComponents[EngineMemoryID][CompVectorIndex] = AllComponents[EngineMemoryID].back();
			AllComponents[EngineMemoryID][CompVectorIndex]->IndexInCompVector = CompVectorIndex;
		}
		AllComponents[EngineMemoryID].pop_back();

		// Finally, deallocate this component
		MemMgr::Free(sizeof(*CompIDPair.second), reinterpret_cast<uint8_t*>(CompIDPair.second));
	}
	
	// Deallocate entity itself
	MemMgr::Free(sizeof(Entity), reinterpret_cast<uint8_t*>(EntityToDestroy));

	// Notify systems
	for (size_t i = 0; i < AllSystems.size(); ++i)
	{
		AllSystems[i]->OnEntityDestroyed(entityID);
	}
}

void Engine::SetCamera(std::shared_ptr<Camera> camera)
{
	CameraPtr = camera;
}

Engine::~Engine()
{
	// Systems
	for (auto SystemCursor = AllSystems.begin(); SystemCursor != AllSystems.end(); ++SystemCursor)
	{
		MemMgr::Free(sizeof(**SystemCursor), reinterpret_cast<uint8_t*>(*SystemCursor));
	}

	// Components - 2D Vector
	for (auto CompTypeCursor = AllComponents.begin(); CompTypeCursor != AllComponents.end(); ++CompTypeCursor)
	{
		for (auto CompCursor = (*CompTypeCursor).begin(); CompCursor != (*CompTypeCursor).end(); ++CompCursor)
		{
			MemMgr::Free(sizeof(**CompCursor), reinterpret_cast<uint8_t*>(*CompCursor));
		}
	}

	// Entities
	for (auto EntityCursor = AllEntities.begin(); EntityCursor != AllEntities.end(); ++EntityCursor)
	{
		MemMgr::Free(sizeof(**EntityCursor), reinterpret_cast<uint8_t*>(*EntityCursor));
	}
}

void Engine::AddAllSystems()
{
	// All systems need to be added here to be updated in the game loop. Their order here is their update order every frame. 
	//AddSystem<MovementSystem>();
	//AddSystem<DamageInRangeSystem>();
	AddSystem<ThrowDartSystem>();
	AddSystem<SkyboxSystem>();
	AddSystem<ShaderSystem>();
	AddSystem<ModelLoadingSystem>();
	AddSystem<RenderingSystem>();
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

	AllocatedComponent->IndexInCompVector = IndexOfNewComponent;
	AllocatedComponent->OwningEntity = entityToAllocateFor.GetID();
	CompMemoryIDMap.emplace(CompType::UniqueID, CompType::EngineMemoryID);

	// Notify systems in case this entity is now interesting to them
	for (size_t i = 0; i < AllSystems.size(); ++i)
	{
		AllSystems[i]->OnEntityCreated(entityToAllocateFor);
	}

	return AllocatedComponent;
}

template<class CompType>
void Engine::DeallocateComponent(Component* compToDeallocate)
{
	unsigned int EngineMemoryID = CompMemoryIDMap[compToDeallocate->UniqueID];
	unsigned int CompVectorIndex = compToDeallocate->IndexInCompVector;

	// Put backmost element of this compType into this CompVectorIndex
	AllComponents[EngineMemoryID][CompVectorIndex] = AllComponents[EngineMemoryID].back();
	AllComponents[EngineMemoryID].pop_back();
	// ..and update the bookkeeping on that component
	AllComponents[EngineMemoryID][CompVectorIndex]->IndexInCompVector = CompVectorIndex;

	// Finally, deallocate this component
	MemMgr::Free(sizeof(*compToDeallocate), reinterpret_cast<uint8_t*>(compToDeallocate));
}

template<class CompType>
void Engine::NotifySystemsOnComponentRemoved(EntityID owningEntity)
{
	for (size_t i = 0; i < AllSystems.size(); ++i)
	{
		// Perf hit because we abstract BaseSystem from System
		AllSystems[i]->OnComponentRemoved(owningEntity, CompType::UniqueID);
	}
}

void Engine::InitTest()
{
	Entity* e1 = CreateEntity();
	Entity* e2 = CreateEntity();
	Entity* e3 = CreateEntity();
	
	//AddSystem<RenderingSystem>();
	//EnableSystem<RenderingSystem>();
	//AddSystem<ShaderSystem>();
	//EnableSystem<ShaderSystem>();
	//AddSystem<ModelLoadingSystem>();
	//EnableSystem<ModelLoadingSystem>();

	///* Transform Testing */
	e1->AddComponent<TransformComponent>();
	TransformComponent* e2_transform = e2->AddComponent<TransformComponent>(); 
	e2_transform->Data = Transform(Vector3(.92303f, .32139f, -.21147f),
		Vector3(-.21147f, .88302f, .41899f),
		Vector3(.32139f, -.34202f, .88302f),
		Vector3(10.f, 10.f, 10.f));
	e2_transform->Data.SetParent(&e1->GetComponent<TransformComponent>()->Data);
	TransformComponent* e3_transform = e3->AddComponent<TransformComponent>();
	e3_transform->Data = e3->GetComponent<TransformComponent>()->Data.Translate(Vector3(10, 10, 10));
	RenderableComponent* cR = e3->AddComponent<RenderableComponent>();
	ShaderComponent* cS = e3->AddComponent<ShaderComponent>();
	CubemapComponent* cCubemap = e3->AddComponent<CubemapComponent>();
	PlayerComponent* cPlayer = e3->AddComponent<PlayerComponent>();

	/* Health Component Testing */
	HealthComponent* e1_health = e1->AddComponent<HealthComponent>();
	e1_health->CurrentHealth = 100.0f;
	e1_health->MaxHealth = e1_health->CurrentHealth;

	HealthComponent* e2_health = e2->AddComponent<HealthComponent>();
	e2_health->CurrentHealth = 100.0f;
	e2_health->MaxHealth = e2_health->CurrentHealth;

	DestroyEntity(e2->GetID());
	DestroyEntity(e1->GetID());
	DestroyEntity(e3->GetID());
}

// initialization is here because Components don't have a .cpp file
unsigned int TransformComponent::EngineMemoryID = UINT32_MAX; 
unsigned int HealthComponent::EngineMemoryID = UINT32_MAX; 
unsigned int CollisionComponent::EngineMemoryID = UINT32_MAX;
unsigned int RenderableComponent::EngineMemoryID = UINT32_MAX;
unsigned int ShaderComponent::EngineMemoryID = UINT32_MAX;
unsigned int CubemapComponent::EngineMemoryID = UINT32_MAX;
unsigned int PlayerComponent::EngineMemoryID = UINT32_MAX;