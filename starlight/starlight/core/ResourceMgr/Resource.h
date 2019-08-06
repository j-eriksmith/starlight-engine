#pragma once

#include <string>

/*
	Author: Jake Smith
	Pure virtual class for a loaded resource, ready to be used in the game.
*/

class Resource
{
public:
	std::string Path;
	std::string Name;
	unsigned int RefCount;
};
