#pragma once
#include "glm/glm.hpp"
#include <iostream>

class Vertex
{
public:
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;

public:
	void print(std::ostream& out) const;

	friend std::ostream& operator << (std::ostream& out, const Vertex& v);

};