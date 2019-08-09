#pragma once
#include <unordered_map>
// TODO: turn on C++17 to get string_view
#include <string>
#include <memory> 

#include "ResourceHandle.h"
#include "../FileIO/FileIO.h"

/*
	Author: Jake Smith
*/
class ResourceMgr
{
public:
	std::shared_ptr<Resource> Load(const std::string& resourceKey);
	void Unload(std::string resourceKey);
	static void AddNewResource(IORequest& request);
private:
	std::unordered_map<std::string, std::weak_ptr<Resource>> Map;
};

