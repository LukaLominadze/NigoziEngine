#pragma once

#include "miniaudio.h"
#include "AudioGroup.h"
#include "Audio.h"

namespace Nigozi
{
	class Application;

	class AudioEngine
	{
	private:
		friend class Application;
		static void Init();
		static void Deinitialize();

		static bool Initialized();
	public:
		static AudioGroup* CreateAudioGroup(const std::string name);
		static void DeleteAudioGroup(AudioGroup* group);
		static void DeleteAudioGroup(const std::string_view& name);

		static AudioGroup* GetAudioGroup(const std::string_view& name);
		static AudioGroup* GetMasterAudioGroup();

		static Audio* LoadAudioFromFile(const std::filesystem::path& filePath);
		static Audio* LoadAudioFromFile(const std::filesystem::path& filePath, const std::string_view& audioGroup);
		static void UnloadAudio(Audio* audio);
	private:
		static ma_engine s_engine;

		static std::vector<AudioGroup> s_audioGroups;
		static std::vector<Audio*> s_audios;

		static bool s_initialized;
	};
}

