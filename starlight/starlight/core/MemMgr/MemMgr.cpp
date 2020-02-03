#include "MemMgr.h"
#include "Debug.h"
// Init our MemMgr singleton to null
std::unique_ptr<MemMgr> MemMgr::memMgr(nullptr);
std::unique_ptr<std::unordered_map<uint8_t*, MemMgr::AllocatorType>> 
MemMgr::addressToAllocatorMap(new std::unordered_map<uint8_t*, MemMgr::AllocatorType>(500000));

MemMgr::MemMgr(uint totalSpace)
	: regionSize(totalSpace),
	start(AllocSpace(regionSize)),
	frameData(new DoubleBufferAllocator(static_cast<unsigned int>(totalSpace * 0.1))),
	poolData(new PoolAllocator(static_cast<unsigned int>(totalSpace * 0.2))),
	globalData(new StackAllocator(static_cast<unsigned int>(totalSpace * 0.1))),
	levelData(new StackAllocator(static_cast<unsigned int>(totalSpace * 0.6)))
{
	// Log("allocated block of size: " << regionSize << "at address: " << start);
}

MemMgr::~MemMgr()
{
	// first delete any references to allocators that you initialized, 
	// then delete the chunk of memory that was allocated at construction.
	delete[] start;
}

uint8_t* MemMgr::Alloc(uint resourceSize, MemMgr::AllocatorType type)
{
	// throw allocation to a specified allocator, depending on the given enum argument
	uint8_t* addr;
	switch (type)
	{
		case MemMgr::AllocatorType::FrameData:
		{
			addr = reinterpret_cast<uint8_t*>(memMgr->frameData->Alloc(resourceSize));
			break;
		}
		case MemMgr::AllocatorType::LevelData:
		{
			addr = reinterpret_cast<uint8_t*>(memMgr->levelData->Alloc(resourceSize));
			break;
		}
		case MemMgr::AllocatorType::GlobalData:
		{
			addr = reinterpret_cast<uint8_t*>(memMgr->globalData->Alloc(resourceSize));
			break;
		}
		case MemMgr::AllocatorType::PoolData:
		{
			addr = reinterpret_cast<uint8_t*>(memMgr->poolData->Alloc(resourceSize));
			break;
		}
	}
	addressToAllocatorMap->emplace(addr, type);
	return addr;
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

void MemMgr::Free(uint sz, uint8_t* addr)
{
	switch (addressToAllocatorMap->at(addr))
	{
		case MemMgr::AllocatorType::FrameData:
		case MemMgr::AllocatorType::LevelData:
		case MemMgr::AllocatorType::GlobalData:
			break;
		case MemMgr::AllocatorType::PoolData:
		{
			memMgr->poolData->Free(sz, addr);
			break;
		}
	}
	addressToAllocatorMap->erase(addr);
}

uint8_t* MemMgr::AllocSpace(uint totalSpace)
{
	// returns array of 'totalSpace' bytes, ready to split into pools
	return new uint8_t[totalSpace];
}





