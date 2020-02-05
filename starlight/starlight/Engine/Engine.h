#pragma once
#include <array>
#include <vector>
#include <limits>

#include "ECSTypes.h"
#include "Entity.h"
#include "System.h"
#include "Component.h"

constexpr unsigned MAX_SYSTEMS = 64; 
constexpr unsigned MAX_COMPONENTS = 32;
constexpr unsigned MAX_ENTITIES = 4096; 

class Engine
{
public:
	explicit Engine() 
		: AllSystems(),
		AllEntities(),
		AllComponents() 
	{
		AddAllSystems();
		InitTest();

		// These initial sizes may need to be emperically adjusted over time.
		// Since our engine targets FPS games, most entities will likely be enviornment props and have few components.
		// (so why did I pick ECS for an FPS engine? good practice... and Overwatch did it!)
		AllSystems.reserve(MAX_SYSTEMS);
		AllComponents.reserve(MAX_COMPONENTS);
		AllEntities.reserve(MAX_ENTITIES);

		for (std::vector<Component*>& compVector : AllComponents)
		{
			compVector.reserve(MAX_ENTITIES);
		}
	}
	void InitTest();
	void Update(float deltaTime);
	Entity* CreateEntity();
	void DestroyEntity(EntityID entityID);

	template <class SystemType>
	void AddSystem(); 
	// Todo(jake): make this private but give Entity the ability to call it
	template <class CompType>
	CompType* AllocateComponent(Entity& entityToAllocateFor);

	~Engine();

private:
	std::vector<BaseSystem*> AllSystems;
	std::vector<Entity*> AllEntities;
	std::vector<std::vector<Component*>> AllComponents;

	void AddAllSystems();
};
