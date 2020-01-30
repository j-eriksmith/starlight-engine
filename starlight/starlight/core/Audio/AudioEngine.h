#pragma once

#include "FMODModule.h"

/*
	User-facing interface for playing and stopping audio tracks.
*/

class AudioEngine
{
public:
	/*
	Loads a sound from a file path into memory to be played later via PlaySound.
	[in]
	soundName: name of audio file from a file path relative to engine executable (likely in the Resources folder)
	is3D: Will the sound change volume and L/R balance as the listener moves around the scene? (default: false)
	looping: Will the sound continuously loop after playing? (default: false)
	stream: If currently playing, will a new PlaySound command on this sound interrupt it? (default: false)
	Example usage: To locate a sound file in the Resources/Audio folder, use Resources::Get("Audio/-sound name here")
	*/
	static void LoadSound(const std::string& soundName, bool is3D = false, bool looping = false, bool stream = false);

	/*
	Plays a sound from a file path, and loads the sound as 2D, non-looping, non-streaming if not loaded already.
	[in]
	soundName: name of audio file from a file path relative to engine executable (likely in the Resources folder)
	pos: if a 3D sound, where in world space will the sound channel come from? (plays in a sphere, disregards walls)
	volumedB: decibel level of the sound to play
	[out]
	The audio channel ID that is currently playing the sound.
	*/
	static int PlaySound(const std::string& soundName, const Vector3& pos = Vector3{ 0, 0, 0 }, float volumedB = 0.0f);

	/*
	Unloads sound from pre-allocated memory.
	[in]
	soundName: name of audio file from a file path relative to engine executable that has been loaded via LoadSound or PlaySound previously
	*/
	static void UnloadSound(const std::string& soundName);

	/*
	Update the 3D position of an audio channel.
	[in]
	channelId: audio channel to update the location of
	position: new world space position
	*/
	static void SetChannel3dPosition(int channelId, const Vector3& position);

	/*
	Update the dB volume of an audio channel.
	[in]
	channelId: audio channel to update the volume of
	volumedB: new decibel level of the channel
	*/
	static void SetChannelVolume(int channelId, float volumedB);

	/*
	Pauses or resumes the selected audio channel
	[in]
	channelId: audio channel to set the paused state for
	pauseState: new pause state for the channel
	*/
	static void SetChannelPaused(int channelId, bool pauseState);

	/*
	Stops channel early and marks the channel for reuse. Will happen automatically for finished, non-looping sounds upon play completion
	[in]
	channelId: audio channel to stop
	*/
	static void StopChannel(int channelId);

	/*
	Checks if channel is currently playing (== not stopped). A paused but not stopped channel is still "playing".
	[in]
	channelId: audio channel to query
	[out]
	Is channel playing (true) or stopped (false)
	*/
	static bool IsPlaying(int channelId);

	static void Initialize(unsigned int audioSize);
	static void Update(const Vector3& listenerPosition, const Vector3& listenerForward);
	static void Shutdown();

private:
	static float dbToVolume(float db);
	static float VolumeTodb(float volume);
	static FMOD_VECTOR VectorToFmod(const Vector3& position);
	static int ErrorCheck(FMOD_RESULT result);

	static FMODModule AudioModule;
};
