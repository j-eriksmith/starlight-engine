#pragma once
#include "Resource.h"
/*
	Author: Jake Smith
	Wraps a resource when requested via string key by the ResourceMgr.
	If resource is ready, the Resource* will be ready.
	If resource is not ready, the Resource* will be null and a NB IOReqeust will be made.
*/
class ResourceHandle
{
public:
	void LoadResource();
	void UnloadResource();

	Resource* Res;
};
