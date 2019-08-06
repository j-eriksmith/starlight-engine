#include "ResourceMgr.h"
#include "../FileIO/FileIO.h"

void ResourceMgr::AddNewResource(IORequest& request)
{
	
}

std::shared_ptr<Resource> ResourceMgr::Load(const std::string& resourceKey)
{
	// Lookup to see if resource is already loaded into memory
	// If so, increment some internal use count and return shared_ptr
	if (Map.contains(resourceKey))
	{
		return Map[resourceKey].lock();
	}
	// Else, throw load request onto IO thread
	// Give some way for the client to wait for this request to be fufilled (optional function?)
	else
	{
		// Todo: figure out how to access where the "Assets" folder is (ie. the whole reason you went on this journey)
		// But first... maybe make sure this workflow works :) 
		IORequest ReadRequest(std::string("C:\\Games\\Starlight-Engine\\starlight\\starlight\\core\\uhh.txt"),
			new byte[65536], 65536, IORequest::READ, AddNewResource);
		FileIO::SubmitRequest(ReadRequest);
	}
}