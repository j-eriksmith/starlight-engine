#pragma once

#include "Component.h"
#include <unordered_map>
#include "Debug.h"
#include "glm/glm.hpp"
#include "String/StringHash.h"

class ShaderComponent;
using ShaderComponentPtr = std::shared_ptr<ShaderComponent>;

class ShaderComponent : public Component
{
public:
	ShaderComponent(unsigned int ID, const glm::mat4& proj = glm::mat4())
		:Component(UniqueID),
		 id(ID), 
		 projectionMatrix(proj) {}

	ShaderComponent()
		:Component(UniqueID),
		 id(0),
		 projectionMatrix(glm::mat4()) {}

	// Note: const ShaderComponent would not allow ShaderSystem to index
	// uniformLocationCache (to enforce const-correctness), but a const member
	// function will allow the ShaderSystem to use the uniformLocationCache.
	// So much for no member fns in components, but if it works it works. One
	// Getter won't be the end of the world :P

	~ShaderComponent() { GLCall(glDeleteProgram(id)); }

	unsigned int id;
	std::unordered_map<std::string, int> uniformLocationCache;
	glm::mat4 projectionMatrix;
	static constexpr ComponentID UniqueID = StringIdHash("ShaderComponent");
	static unsigned int EngineMemoryID;
};

struct ShaderProgramSource
{
	ShaderProgramSource()
	{}
	ShaderProgramSource(std::string vs, std::string fs)
		:VertexShader(vs), FragmentShader(fs)
	{}
	ShaderProgramSource(const ShaderProgramSource& rhs)
		:VertexShader(rhs.VertexShader),
		 FragmentShader(rhs.FragmentShader)
	{}
	std::string VertexShader;
	std::string FragmentShader;
};
