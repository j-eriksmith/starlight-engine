#pragma once
#include <unordered_map>
#include "ECSTypes.h"
#include "Component.h"

class Entity
{
	using CompMap = std::unordered_map<ComponentID, Component*>;

	EntityID ID;
	CompMap Components;
public:
	explicit Entity(EntityID id)
		: ID(id) {}

	// Copy constructor/assignment bad, move construct/assign is fine
	Entity(const Entity&) = delete;
	Entity(Entity&&) = default;
	Entity& operator=(const Entity&) = delete;
	Entity& operator=(Entity&&) = default;
	~Entity() = default;

	//template <class CompType>
	void AddComponent(ComponentID compID, Component* comp)
	{
		// Todo(jake): assert that the entity doesn't already have this component

		Components.emplace(compID, comp);
		// Todo(jake): Decide if this should be a templated class where the entity puts the component into memory
		// or if we should have the developer know where to put components
		// this function would be a lot simpler to call if we could!
		// Components.emplace(CompType::ID, new CompType);
	}
	
	void RemoveComponent(ComponentID compID)
	{
		Components.erase(compID);
	}

	EntityID GetID() const { return ID; }

	const CompMap& GetComponents() const
	{
		return Components;
	}

	template<class CompType>
	CompType* GetComponent()
	{
		auto FoundComponent = Components.find(CompType::ID);
		return FoundComponent != Components.end() ? dynamic_cast<CompType*>(FoundComponent->second) : nullptr;
	}
};
