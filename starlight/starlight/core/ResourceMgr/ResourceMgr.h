#pragma once
#include <unordered_map>
// TODO: turn on C++17 to get string_view
#include <string>
#include <memory> 

#include "ResourceHandle.h"

/*
	Author: Jake Smith
*/
class ResourceMgr
{
public:
	std::shared_ptr<Resource> Load(std::string resourceKey);
	void Unload(std::string resourceKey);
private:
	std::unordered_map<std::string, std::weak_ptr<Resource>> Map;
};

