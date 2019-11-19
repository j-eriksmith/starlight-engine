#pragma once
#include "Entity.h"
#include "BaseSystem.h"
#include <vector>
#include <unordered_map>


template <class... Comps>
class System : public BaseSystem
{
	using EntityIDtoIndexMap = std::unordered_map<EntityID, size_t>;
	EntityIDtoIndexMap EntityIDMap;
protected:
	// Needs a reference to every component that it cares about
	using CompTuple = std::tuple<std::add_pointer_t<Comps>...>; // Tuple of component pointers
	std::vector<CompTuple> Components; // Todo(jake): needs to be pool allocated

public:
	explicit System(Engine* pEntityEngine)
		: BaseSystem(pEntityEngine) {}

	virtual void OnEntityCreated(const Entity& entity) override final;
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
inline void System<Comps...>::OnEntityCreated(const Entity& entity)
{
	CompTuple CompTuple;
	size_t Matches = 0;
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
	size_t Index = EntityEntry->second;
	if (EntityEntry != EntityIDMap.end())
	{
		Components[Index] = std::move(Components.back()); // delete via moving last component to vacant index
		Components.pop_back(); // bookkeep the moved component

		// The entity that owned the moved component needs to update it's bookkeeping
		// For the type of component removed, get the first component we have
		// Components[Index] = which entity
		// std::get<0> = first component for that entity
		auto* MovedComponent = std::get<0>(Components[Index]);

		// Use the first component to lookup the EntityID in our map
		// auto MovedTupleIterator = EntityIDMap.find(MovedComponent->GetEntityID());
		// MovedTupleIterator->second = Index;
		EntityIDMap.find(MovedComponent->GetEntityID())->second = Index;
	}
	else
	{
		// Shouldn't happen?	
	}
}

template<class ...Comps>
template<size_t index, class CompType, class ...CompArgs>
inline bool System<Comps...>::ProcessComponent(ComponentID compID, Component* component, CompTuple& tupleToFill)
{
	if (CompType::ID == compID)
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
