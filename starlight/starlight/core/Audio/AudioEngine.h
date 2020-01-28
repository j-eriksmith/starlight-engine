#pragma once

#include "FMODModule.h"

class AudioEngine
{
public:
	static void Update(const Vector3& listenerPosition, const Vector3& listenerForward);
	static int ErrorCheck(FMOD_RESULT result);

	static void LoadSound(const std::string& soundName, bool is3D = false, bool looping = false, bool stream = false);
	static int PlaySound(const std::string& soundName, const Vector3& pos = Vector3{ 0, 0, 0 }, float volumedB = 0.0f);
	static void UnloadSound(const std::string& soundName);

	static void SetChannel3dPosition(int channelId, const Vector3& position);
	static void SetChannelVolume(int channelId, float volumedB);
	static void SetChannelPaused(int channelId, bool pauseState);
	static void StopChannel(int channelId);
	static bool IsPlaying(int channelId);

private:
	static float dbToVolume(float db);
	static float VolumeTodb(float volume);
	static FMOD_VECTOR VectorToFmod(const Vector3& position);
	static FMODModule AudioModule;
};
