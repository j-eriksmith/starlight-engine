#include "ResourceMgr.h"
#include <functional>
#include <string>

std::future<std::shared_ptr<Texture>> ResourceMgr::Load(const std::string& request)
{
	std::promise<std::shared_ptr<Texture>> TexturePromise;
	std::string ResourceKey = Resources::Get(request);

	// Lookup to see if resource is already loaded into memory
	// If so, return loaded resource
	if (Map.find(ResourceKey) != Map.end() && !Map[ResourceKey].expired())
	{
		TexturePromise.set_value(Map[ResourceKey].lock());
		return TexturePromise.get_future();
	}

	// Else, throw load request onto IO thread
	else
	{
		// Todo(jake): Use Jandro's mem mgr once statically accessible 
		TextureRequest* LoadTexture = new TextureRequest(ResourceKey, std::bind(&ResourceMgr::AddNewResource, this, std::placeholders::_1));
		FileIO::SubmitRequest(LoadTexture);
		return LoadTexture->TexturePromise.get_future();
	}
}

void ResourceMgr::Unload(const std::string& request)
{
	std::string ResourceKey = Resources::Get(request);
	if (Map.find(ResourceKey) == Map.end())
	{
		printf("Fatal! Couldn't find resource \"%s\" to unload from resource map!\n", ResourceKey.c_str());
	}

	if (Map[ResourceKey].use_count() - 1 == 0)
	{
		Map.erase(ResourceKey);
	}
}

void ResourceMgr::AddNewResource(FileRequest* request)
{
	std::shared_ptr<Texture> TexturePtr; // saves a double map lookup
	TextureRequest* textureRequest = dynamic_cast<TextureRequest*>(request);
	Map[request->FileName] = TexturePtr = std::make_shared<Texture>(*(textureRequest->LoadedTexture)); // Hack to get this to compile
	textureRequest->TexturePromise.set_value(TexturePtr);
}
