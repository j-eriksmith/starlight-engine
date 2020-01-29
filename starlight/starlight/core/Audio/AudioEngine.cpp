#include "AudioEngine.h"
#include "math/utils.h"
#include "Debug.h"
#include "String/StringHash.h"

FMODModule AudioEngine::AudioModule{};

void AudioEngine::Initialize(unsigned int audioSize)
{
	AudioModule.Initialize(audioSize);
}

void AudioEngine::Update(const Vector3& listenerPosition, const Vector3& listenerForward)
{
	// Calculate up global vector so user doesn't have to maintain it
	Vector3 RightVector = Vector3(0.f, 1.f, 0.f).Cross(listenerForward);
	Vector3 UpVector = listenerForward.Cross(RightVector).Normalized();

	// Based on listener position, forward, up vector, update FMOD 3d Listener
	AudioEngine::ErrorCheck(AudioModule.CoreSystem->set3DListenerAttributes(0,
		&VectorToFmod(listenerPosition),
		nullptr, // only needed for doppler effect
		&VectorToFmod(listenerForward),
		&VectorToFmod(UpVector)));

	AudioModule.Update();
}

void AudioEngine::Shutdown()
{
	AudioModule.Shutdown();
}

int AudioEngine::ErrorCheck(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		Log("FMOD Error with FMOD_Result: ");
		Log(result);
		return 1;
	}
	return 0;
}

void AudioEngine::LoadSound(const std::string& soundName, bool is3D, bool looping, bool stream)
{
	StringId HashedSoundName = SID(soundName.c_str());
	auto FoundSound = AudioModule.Sounds.find(HashedSoundName);
	if (FoundSound != AudioModule.Sounds.end())
	{
		// Sound is already loaded, so return
		return;
	}

	FMOD_MODE Mode = FMOD_DEFAULT;
	// Set bits in FMOD_MODE (unsigned int) depending on parameters
	Mode |= is3D ? FMOD_3D : FMOD_2D;
	Mode |= looping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
	Mode |= stream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;

	// Attempt to load the sound
	FMOD::Sound* Sound = nullptr;
	AudioEngine::ErrorCheck(AudioModule.CoreSystem->createSound(soundName.c_str(), Mode, nullptr, &Sound));
	if (Sound != nullptr)
	{
		// Put the created sounds into our map
		AudioModule.Sounds[HashedSoundName] = Sound;
	}
}

void AudioEngine::UnloadSound(const std::string& soundName)
{
	auto FoundSound = AudioModule.Sounds.find(SID(soundName.c_str()));
	if (FoundSound == AudioModule.Sounds.end())
	{
		// Sound is not loaded, so doesn't make sense to unload
		return;
	}

	AudioEngine::ErrorCheck(FoundSound->second->release());
	AudioModule.Sounds.erase(FoundSound);
}

int AudioEngine::PlaySound(const std::string& soundName, const Vector3& pos, float volumedB)
{
	StringId HashedSoundName = SID(soundName.c_str());
	int CurrentChannelID = AudioModule.nextChannelID++;

	// Ensure that desired sound is loaded
	auto FoundSound = AudioModule.Sounds.find(HashedSoundName);
	if (FoundSound == AudioModule.Sounds.end())
	{
		LoadSound(soundName);
		FoundSound = AudioModule.Sounds.find(HashedSoundName);
		// There was an error during sound loading
		if (FoundSound == AudioModule.Sounds.end())
		{
			return CurrentChannelID;
		}
	}

	FMOD::Channel* ChannelToUse = nullptr;
	AudioEngine::ErrorCheck(AudioModule.CoreSystem->playSound(FoundSound->second, nullptr, true, &ChannelToUse));
	if (ChannelToUse != nullptr)
	{
		// Set 3D location if the sound was loaded in for 3D
		FMOD_MODE FoundSoundMode;
		FoundSound->second->getMode(&FoundSoundMode);
		if (FoundSoundMode & FMOD_3D)
		{
			FMOD_VECTOR SoundPosition = VectorToFmod(pos);
			AudioEngine::ErrorCheck(ChannelToUse->set3DAttributes(&SoundPosition, nullptr));
		}

		// Successfully played sound via ChannelToUse... now configure volume before playing
		AudioEngine::ErrorCheck(ChannelToUse->setVolume(dbToVolume(volumedB)));
		AudioEngine::ErrorCheck(ChannelToUse->setPaused(false)); // paused in the playSound call
		AudioModule.Channels[CurrentChannelID] = ChannelToUse;
	}
	return CurrentChannelID;
}

float AudioEngine::dbToVolume(float dB)
{
	return Utils::Pow(10.0f, 0.05f * dB);
}

float AudioEngine::VolumeTodb(float volume)
{
	return 20.0f * Utils::Log10(volume);
}

FMOD_VECTOR AudioEngine::VectorToFmod(const Vector3& position)
{
	FMOD_VECTOR ReturnVector;
	ReturnVector.x = position.x;
	ReturnVector.y = position.y;
	ReturnVector.z = position.z;
	return ReturnVector;
}

void AudioEngine::SetChannel3dPosition(int channelId, const Vector3& position)
{
	auto FoundChannel = AudioModule.Channels.find(channelId);
	if (FoundChannel == AudioModule.Channels.end())
	{
		return;
	}
	FMOD_VECTOR Position = VectorToFmod(position);
	AudioEngine::ErrorCheck(FoundChannel->second->set3DAttributes(&VectorToFmod(position), nullptr));
}

void AudioEngine::SetChannelVolume(int channelId, float volumedB)
{
	auto FoundChannel = AudioModule.Channels.find(channelId);
	if (FoundChannel == AudioModule.Channels.end())
	{
		return;
	}

	AudioEngine::ErrorCheck(FoundChannel->second->setVolume(dbToVolume(volumedB)));
}

void AudioEngine::SetChannelPaused(int channelId, bool pauseState)
{
	auto FoundChannel = AudioModule.Channels.find(channelId);
	if (FoundChannel == AudioModule.Channels.end())
	{
		return;
	}

	AudioEngine::ErrorCheck(FoundChannel->second->setPaused(pauseState));
}

void AudioEngine::StopChannel(int channelId)
{
	auto FoundChannel = AudioModule.Channels.find(channelId);
	if (FoundChannel == AudioModule.Channels.end())
	{
		return;
	}

	AudioEngine::ErrorCheck(FoundChannel->second->stop());
}

bool AudioEngine::IsPlaying(int channelId)
{
	auto FoundChannel = AudioModule.Channels.find(channelId);
	if (FoundChannel == AudioModule.Channels.end())
	{
		return false;
	}

	bool IsPlaying;
	AudioEngine::ErrorCheck(FoundChannel->second->isPlaying(&IsPlaying));
	return IsPlaying;
}
