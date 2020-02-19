#pragma once
#include "Component.h"
#include "Mesh.h"
#include "BoundingBox.h"
#include <vector>
#include <memory>
#include "String/StringHash.h"

class RenderableComponent : public Component
{
public:
	explicit RenderableComponent()
		:Component(),
		meshes(std::vector<std::shared_ptr<Mesh>>()),
		bb(BoundingBoxPrimitives()),
		shouldRender(true)
	{}

	RenderableComponent(std::vector<std::shared_ptr<Mesh>> m, BoundingBoxPrimitives primitives, bool render)
		:Component(),
		meshes(m),
		bb(primitives),
		shouldRender(render)
	{}

	static constexpr ComponentID UniqueID = StringIdHash("RenderableComponent");
	static unsigned int EngineMemoryID;

	std::vector<std::shared_ptr<Mesh>> meshes;
	BoundingBoxPrimitives bb;
	bool shouldRender;
};