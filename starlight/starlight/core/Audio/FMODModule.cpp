#include "FMODModule.h"
#include "AudioEngine.h"
#include "MemMgr.h"

void FMODModule::Initialize(unsigned int audioSize)
{
	// Allocate memory
	AllocatedAudioMemory = MemMgr::Alloc(audioSize, MemMgr::AllocatorType::LevelData);
	AudioAllocationSize = audioSize;

	AudioEngine::ErrorCheck(FMOD::Memory_Initialize(AllocatedAudioMemory, audioSize, nullptr, nullptr, nullptr));

	// Init studio system
	AudioEngine::ErrorCheck(FMOD::Studio::System::create(&StudioSystem));
	AudioEngine::ErrorCheck(StudioSystem->initialize(32, FMOD_STUDIO_INIT_LIVEUPDATE, FMOD_INIT_PROFILE_ENABLE | FMOD_INIT_3D_RIGHTHANDED, nullptr));

	// Init core system
	AudioEngine::ErrorCheck(StudioSystem->getCoreSystem(&CoreSystem));

	// Set one 3D Listener
	AudioEngine::ErrorCheck(CoreSystem->set3DNumListeners(1));
}

void FMODModule::Shutdown()
{
	AudioEngine::ErrorCheck(StudioSystem->unloadAll());
	AudioEngine::ErrorCheck(StudioSystem->release()); 
	MemMgr::Free(AudioAllocationSize, AllocatedAudioMemory);
}

void FMODModule::Update()
{
	// Find and clear stopped channels
	static std::vector<ChannelMap::iterator> StoppedChannels;
	for (auto it = Channels.begin(), itEnd = Channels.end(); it != itEnd; ++it)
	{
		bool IsPlaying = false;
		it->second->isPlaying(&IsPlaying);
		if (!IsPlaying)
		{
			// Collect unused channel
			StoppedChannels.push_back(it);
		}
	}

	// Free unused channels for further use
	for (auto& it : StoppedChannels)
	{
		Channels.erase(it);
	}
	AudioEngine::ErrorCheck(StudioSystem->update());
	StoppedChannels.clear();
}
