#pragma once
#include <limits>
#include "ECSTypes.h"

/*
	Guide to setting up a derived Component:
	1) Each component needs a `static unsigned int EngineMemoryID` member. 
		Initialize this at the bottom of Engine.cpp to UINT32_MAX.
	2) Each component needs a `static constexpr ComponentID UniqueID` member that its base class is constructed with. 
		Initialize this in the header file (I suggest using StringHash.h's function `StringIdHash` on the component name)
	3) Each component requires a default empty constructor. Initialize data members from the pointer
		returned from AddComponent or write an initialization method for your component.
	4) Ideally, components would be plain-old-data members (ie. no pointers to other memory locations) and their constructors. 
		This is to ensure that when a system needs to process components, it's not jumping around main memory and pulling in many other locations
		into the cache besides the data it needs to operate on. 
	5) Ideally, there would also be no member functions. Systems handle and transform data, not components on themselves.
*/

class Component
{
public:
	explicit Component(ComponentID IDfromSubclass)
		: UniqueID(IDfromSubclass),
		IndexInCompVector(UINT32_MAX),
		OwningEntity(UINT32_MAX) {}
	Component(const Component& c) = delete;
	Component operator=(const Component& c) = delete;

	virtual ComponentID GetUniqueID() { return UniqueID; }
	ComponentID UniqueID;
	EntityID OwningEntity;

protected:
	unsigned int IndexInCompVector; // per-instance index into this Component-type's vector in Engine
	friend class Engine;
};

