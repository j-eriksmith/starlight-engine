#pragma once
#include "Entity.h"
#include "BaseSystem.h"
#include "MemMgr.h"
#include <vector>
#include <unordered_map>

/*
	Guide to setting up a derived System:

	-1) Your derivation must inherit from System, NOT BaseSystem. 
	0) Add your System type to Engine.cpp's AddAllSystems function.
	1) Default constructor initializes its parent type with nullptr
	2) Engine* constructor initializes its parent type with the Engine*
	3) Override BaseSystem's virtual function Update. 
		On each call of this function, iterate through the vector Components. Each item in Components contains a tuple of components,
		corresponding to one entity that has all components that the system cares about. 
	4) Ideally, systems would have only member functions, not member data. 
	In ECS, all data that the system needs to be operated on is handed to them via the components they care about.
*/

template <class... Comps>
class System : public BaseSystem
{
	using EntityIDtoIndexMap = std::unordered_map<EntityID, size_t>;
	EntityIDtoIndexMap EntityIDMap;
protected:
	// Needs a reference to every component that it cares about
	using CompTuple = std::tuple<std::add_pointer_t<Comps>...>; // Tuple of component pointers
	std::vector<CompTuple> Components; // Should be pool allocated, but we have no way of allocating std::collections, just the pointed-to objects they contain.
	bool Enabled;

public:
	explicit System()
		: BaseSystem(nullptr) {}
	System(Engine* pEntityEngine)
		: BaseSystem(pEntityEngine) {}
	
	virtual void OnEntityCreated(Entity& entity) override final;
	virtual void OnEntityDestroyed(EntityID entity) override final;
	virtual void OnComponentRemoved(EntityID entity, ComponentID compID) override final;

private:
	template<class CompType1, class CompType2, class ...CompArgs>
	bool FindComponentForEntity(ComponentID compID);

	template<class CompType>
	bool FindComponentForEntity(ComponentID compID);

	// recursive search through component types
	template<size_t index, class CompType, class ...CompArgs>
	bool ProcessComponent(ComponentID compID, Component* component, CompTuple& tupleToFill);

	// termination of the recursion == no CompType passed
	template<size_t index>
	bool ProcessComponent(ComponentID compID, Component* component, CompTuple& tupleToFill);
};

template<class ...Comps>
inline void System<Comps...>::OnEntityCreated(Entity& entity)
{
	CompTuple CompTuple;
	size_t Matches = 0;

	// Since this function is also called when a new component is dynamically added,
	// we'll check to make sure that the system isn't already aware of this entity
	if (EntityIDMap.find(entity.GetID()) != EntityIDMap.end())
	{
		return;
	}

	// Iterate through components of entity and check if this system cares about it now
	for (auto& CompPair : entity.GetComponents())
	{
		if (ProcessComponent<0, Comps...>(CompPair.first, CompPair.second, CompTuple))
		{
			++Matches;
			if (Matches == sizeof...(Comps))
			{
				Components.emplace_back(std::move(CompTuple));
				EntityIDMap.emplace(entity.GetID(), Components.size() - 1);
				break;
			}
		}
	}
}

template<class ...Comps> 
inline void System<Comps...>::OnComponentRemoved(EntityID entity, ComponentID compID)
{
	// Since this function is called when a component is dynamically removed,
	// exit early if the system doesn't care about the corresponding entity
	if (EntityIDMap.find(entity) == EntityIDMap.end())
	{
		return;
	}

	unsigned int IDinCompVector = EntityIDMap[entity];
	// This entity can still be valid - recur to check if this system cared about this comp in the first place
	// because System Cared About Entity + System Cared About Component + Entity No Longer Has Component = Entity evicted
	if (FindComponentForEntity<Comps...>(compID))
	{
		// evict entity as if it was destroyed
		const auto EntityEntry = EntityIDMap.find(entity);
		size_t Index = EntityEntry->second;

		// if not the last component, delete via moving last component to vacant index
		if (Index + 1 < Components.size())
		{
			Components[Index] = std::move(Components.back()); 

			// For the entity we just moved, update our map to indicate at what index the entity's components now live
			auto* MovedComponent = std::get<0>(Components[Index]);

			// Use the first component to lookup the EntityID in our map
			EntityIDMap.find(MovedComponent->OwningEntity)->second = Index;
		}
		Components.pop_back(); 
		EntityIDMap.erase(EntityEntry);
	}
}

template<class ...Comps>
inline void System<Comps...>::OnEntityDestroyed(EntityID entity)
{
	const auto EntityEntry = EntityIDMap.find(entity);
	if (EntityEntry != EntityIDMap.end())
	{
		size_t Index = EntityEntry->second;
		// if not the last component, delete via moving last component to vacant index
		if (Index + 1 < Components.size())
		{
			Components[Index] = std::move(Components.back()); 

			// For the entity we just moved, update our map to indicate at what index the entity's components now live
			auto* MovedComponent = std::get<0>(Components[Index]);

			// Use the first component to lookup the EntityID in our map
			EntityIDMap.find(MovedComponent->OwningEntity)->second = Index;
		}
		Components.pop_back();
		EntityIDMap.erase(EntityEntry);
	}
	else
	{
		// This system doesn't care about this entity, so don't do anything
	}
}

template<class ...Comps>
template<class CompType1, class CompType2, class ...CompArgs>
inline bool System<Comps...>::FindComponentForEntity(ComponentID compID)
{
	if (FindComponentForEntity<CompType1>(compID))
	{
		return true;
	}
	return FindComponentForEntity<CompType2, CompArgs...>(compID);
}

template<class ...Comps>
template<class CompType>
inline bool System<Comps...>::FindComponentForEntity(ComponentID compID)
{
	return CompType::UniqueID == compID;
}

template<class ...Comps>
template<size_t index, class CompType, class ...CompArgs>
inline bool System<Comps...>::ProcessComponent(ComponentID compID, Component* component, CompTuple& tupleToFill)
{
	if (CompType::UniqueID == compID)
	{
		// We have the type we're looking for
		std::get<index>(tupleToFill) = static_cast<CompType*>(component);
		return true;
	}
	else
	{
		// We iterate one deeper into the list with the rest of the components
		return ProcessComponent<index + 1, CompArgs...>(compID, component, tupleToFill);
	}
}

template<class ...Comps>
template<size_t index>
inline bool System<Comps...>::ProcessComponent(ComponentID compID, Component* component, CompTuple& tupleToFill)
{
	return false;
}
