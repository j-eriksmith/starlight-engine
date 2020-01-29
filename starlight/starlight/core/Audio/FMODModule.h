#pragma once

#ifndef _AUDIO_ENGINE_H
#define _AUDIO_ENGINE_H

#include "Dependencies/FMOD/studio/inc/fmod_studio.hpp"
#include "Dependencies/FMOD/core/inc/fmod.hpp"
#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include <iostream>
#include "math/vec3.h"
#include "String/StringHash.h"

#endif

/*
	Responsible for initializing and shutting down the FMOD engine.
	Holds instances to the Studio and Core system objects.
*/
class FMODModule
{
public:
	FMODModule() = default;
	~FMODModule() = default;

	void Initialize(unsigned int audioSize);
	void Update();
	void Shutdown();
private:
	FMOD::Studio::System* StudioSystem;
	FMOD::System* CoreSystem;
	uint8_t* AllocatedAudioMemory;
	unsigned int AudioAllocationSize;
	int nextChannelID;

	typedef std::unordered_map<StringId, FMOD::Sound*> SoundMap;
	typedef std::map<int, FMOD::Channel*> ChannelMap;

	SoundMap Sounds;
	ChannelMap Channels;

	friend class AudioEngine;
};
