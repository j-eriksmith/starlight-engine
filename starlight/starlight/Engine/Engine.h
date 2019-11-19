#pragma once
#include <array>
#include <vector>

#include "ECSTypes.h"
#include "Entity.h"
#include "System.h"
#include "Component.h"

// Todo(jake): Somehow determine how many systems have been written
constexpr unsigned NUM_SYSTEMS = 1;
constexpr unsigned MAX_GAMEOBJECTS = 4096; // Unused, unsure of how to best bound dynamic memory from users

class Engine
{
public:
	explicit Engine() 
		: Systems(),
		AllEntities(),
		AllComponents() 
	{
		InitTest();
	}
	void InitTest();
	void Update();
	void MainLoop();
	void AddSystem(BaseSystem* inSystem); 
	void AddEntity(Entity& entityToAdd);
private:
	std::vector<BaseSystem*> Systems;
	std::vector<Entity> AllEntities;
	std::vector<Component> AllComponents;
};
