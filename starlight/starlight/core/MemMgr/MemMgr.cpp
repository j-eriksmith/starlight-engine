#include "MemMgr.h"
#include "Debug.h"
// Init our MemMgr singleton to null
std::unique_ptr<MemMgr> MemMgr::memMgr(nullptr);


MemMgr::MemMgr(uint totalSpace)
	: regionSize(totalSpace),
	start(AllocSpace(regionSize)),
	frameData(new DoubleBufferAllocator(totalSpace * 0.1)),
	poolData(new PoolAllocator(totalSpace * 0.2)),
	globalData(new StackAllocator(totalSpace * 0.1)),
	levelData(new StackAllocator(totalSpace * 0.6))
{
	// Log("allocated block of size: " << regionSize << "at address: " << start);
}

MemMgr::~MemMgr()
{
	// first delete any references to allocators that you initialized, 
	// then delete the chunk of memory that was allocated at construction.
	delete[] start;
}

MemoryResource* MemMgr::Alloc(uint resourceSize, MemMgr::AllocatorType type)
{
	// throw allocation to a specified allocator, depending on the given enum argument
	MemoryResource* res = reinterpret_cast<MemoryResource*>(memMgr->poolData->Alloc(sizeof(MemoryResource)));
	res->type = type;
	switch (type)
	{
		case MemMgr::AllocatorType::FrameData:
		{
			res->addr = reinterpret_cast<uint8_t*>(memMgr->frameData->Alloc(resourceSize));
			break;
		}
		case MemMgr::AllocatorType::LevelData:
		{
			res->addr = reinterpret_cast<uint8_t*>(memMgr->levelData->Alloc(resourceSize));
			break;
		}
		case MemMgr::AllocatorType::GlobalData:
		{
			res->addr = reinterpret_cast<uint8_t*>(memMgr->globalData->Alloc(resourceSize));
			break;
		}
		case MemMgr::AllocatorType::PoolData:
		{
			res->addr = reinterpret_cast<uint8_t*>(memMgr->poolData->Alloc(resourceSize));
			break;
		}
	}
	return res;
}

void MemMgr::Create(uint totalSpace)
{
	if (memMgr)
	{
		Log("MemMgr::Create -- MemMgr singleton instance already initialized.");
	}
	else
	{
		memMgr.reset(new MemMgr(totalSpace));
	}

}

void MemMgr::Free(MemoryResource* res)
{
	switch (res->type)
	{
		case MemMgr::AllocatorType::FrameData:
		case MemMgr::AllocatorType::LevelData:
		case MemMgr::AllocatorType::GlobalData:
			break;
		case MemMgr::AllocatorType::PoolData:
		{
			memMgr->poolData->Free(res->size, res->addr);
			break;
		}
	}
}

uint8_t* MemMgr::AllocSpace(uint totalSpace)
{
	// returns array of 'totalSpace' bytes, ready to split into pools
	return new uint8_t[totalSpace];
}





