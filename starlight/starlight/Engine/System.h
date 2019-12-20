#pragma once
#include "Entity.h"
#include "BaseSystem.h"
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

public:
	explicit System()
		: BaseSystem(nullptr) {}
	explicit System(Engine* pEntityEngine)
		: BaseSystem(pEntityEngine) {}

	virtual void OnEntityCreated(Entity& entity) override final;
	virtual void OnEntityDestroyed(EntityID entity) override final;

private:
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
inline void System<Comps...>::OnEntityDestroyed(EntityID entity)
{
	const auto EntityEntry = EntityIDMap.find(entity);
	if (EntityEntry != EntityIDMap.end())
	{
		size_t Index = EntityEntry->second;
		Components[Index] = std::move(Components.back()); // delete via moving last component to vacant index
		Components.pop_back(); // bookkeep the moved component

		// The entity that owned the moved component needs to update it's bookkeeping
		// For the type of component removed, get the first component we have
		// Components[Index] = which entity
		// std::get<0> = first component for that entity
		auto* MovedComponent = std::get<0>(Components[Index]);

		// Use the first component to lookup the EntityID in our map
		// auto MovedTupleIterator = EntityIDMap.find(MovedComponent->GetUniqueID());
		// MovedTupleIterator->second = Index;
		EntityIDMap.find(MovedComponent->GetUniqueID())->second = Index;
	}
	else
	{
		// This system doesn't care about this entity, so don't do anything
	}
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
