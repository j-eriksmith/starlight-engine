#pragma once
#include <unordered_map>
// TODO(jake): turn on C++17 to get string_view
#include <string>
#include <memory> 
#include <future>

#include "Resources.h"
#include "Texture.h"
#include "FileIO/FileIO.h"

/*
	Author: Jake Smith
*/
class ResourceMgr
{
public:
	ResourceMgr()
		: Map() {}

	// A Renderable component asks for a texture by passing the file name.
	std::future<std::shared_ptr<Texture>> Load(const std::string& resourceKey);
	
	// When a Renderable component dies, notify the resource manager to delete
	// the key from the map if shared_ptr is now expired.
	void Unload(const std::string& resourceKey); 
private:
	std::unordered_map<std::string, std::weak_ptr<Texture>> Map;
	void AddNewResource(FileRequest* request);
};

