#pragma once
#include <unordered_map>
#include "ECSTypes.h"
#include "Component.h"
#include "Debug.h"

class Engine;

class Entity
{
	using CompMap = std::unordered_map<ComponentID, Component*>; // Comp Unique ID : Allocated Component

	Engine* EntityEngine;
	EntityID ID;
	CompMap Components;
public:
	explicit Entity(Engine* pEntityEngine, EntityID id)
		: EntityEngine(pEntityEngine),
		ID(id),
		Components() {}

	// Copy constructor/assignment bad, move construct/assign is fine
	Entity(const Entity&) = delete;
	Entity(Entity&&) = default;
	Entity& operator=(const Entity&) = delete;
	Entity& operator=(Entity&&) = default;
	~Entity() = default;

	template <class CompType>
	CompType* AddComponent()
	{
		if (Components.find(CompType::UniqueID) != Components.end())
		{
			Log("Error AddComponent: Entity ID " + std::to_string(ID) + " already has this component: " + std::to_string(CompType::UniqueID));
		}

		CompType* AllocatedComponent = EntityEngine->AllocateComponent<CompType>(*this);
		return AllocatedComponent;
	}

	template <class CompType>
	void RemoveComponent()
	{
		if (Components.find(CompType::UniqueID) == Components.end())
		{
			Log("Error RemoveComponent: Entity ID " + std::to_string(ID) + " does not have this component: " + std::to_string(CompType::UniqueID));
		}
		EntityEngine->NotifySystemsOnComponentRemoved<CompType>(ID);
		EntityEngine->DeallocateComponent<CompType>(Components.find(CompType::UniqueID)->second);
	}
	
	void RemoveComponent(ComponentID compID)
	{
		Components.erase(compID);
	}

	EntityID GetID() const { return ID; }

	// I originally had this as const, but then Engine wanted to call it. I feel like making the member a friend is in order.
	CompMap& GetComponents()
	{
		return Components;
	}

	template<class CompType>
	CompType* GetComponent()
	{
		auto FoundComponent = Components.find(CompType::UniqueID);
		return FoundComponent != Components.end() ? dynamic_cast<CompType*>(FoundComponent->second) : nullptr;
	}
};
