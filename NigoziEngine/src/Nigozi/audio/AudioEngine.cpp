#include "ngpch.h"
#include "AudioEngine.h"

namespace Nigozi
{
	constexpr uint32_t MASTER_AUDIO_GROUP_INDEX = 0;
	constexpr uint32_t MAX_AUDIO_GROUPS = 32;

	ma_engine AudioEngine::s_engine;
	std::vector<AudioGroup> AudioEngine::s_audioGroups;
	std::vector<Audio*> AudioEngine::s_audios;
	bool AudioEngine::s_initialized = false;

	void AudioEngine::Init()
	{
		ma_result result;

		result = ma_engine_init(NULL, &s_engine);
		if (result != MA_SUCCESS) {
			// Failed to initialize the engine.
			std::cout << "Failed to initialize audio engine! : " << result << std::endl;
			s_initialized = false;
			return;
		}

		s_audioGroups.reserve(MAX_AUDIO_GROUPS);
		s_audioGroups.emplace_back(s_engine, "Master");
		s_initialized = true;
	}

	void AudioEngine::Deinitialize()
	{
		for (Audio* audio : s_audios) {
			delete audio;
		}
		s_audios.clear();
		s_audioGroups.clear();
		ma_engine_uninit(&s_engine);
	}

	bool AudioEngine::Initialized()
	{
		return s_initialized;
	}


	AudioGroup* AudioEngine::CreateAudioGroup(const std::string name)
	{
		if (s_audioGroups.size() >= MAX_AUDIO_GROUPS) {
			return nullptr;
		}
		auto it = std::find_if(s_audioGroups.begin(), s_audioGroups.end(), [name](AudioGroup& group) {
			return name == group.GetName();
			});
		if (it != s_audioGroups.end()) {
			return nullptr;
		}
		s_audioGroups.emplace_back(s_engine, name, s_audioGroups[MASTER_AUDIO_GROUP_INDEX]);
		return &(*--s_audioGroups.end());
	}

	void AudioEngine::DeleteAudioGroup(AudioGroup* group)
	{
		auto it = std::find_if(s_audioGroups.begin(), s_audioGroups.end(), [group](AudioGroup& _group) {
			return group == &_group;
			});
		if (it == s_audioGroups.end()) {
			return;
		}
		s_audioGroups.erase(it);
	}

	void AudioEngine::DeleteAudioGroup(const std::string_view& name)
	{
		auto it = std::find_if(s_audioGroups.begin(), s_audioGroups.end(), [name](AudioGroup& group) {
			return name == group.GetName();
			});
		if (it == s_audioGroups.end()) {
			return;
		}
		s_audioGroups.erase(it);
	}


	AudioGroup* AudioEngine::GetAudioGroup(const std::string_view& name)
	{
		auto it = std::find_if(s_audioGroups.begin(), s_audioGroups.end(), [name](AudioGroup& group) {
			return name == group.GetName();
			});
		if (it == s_audioGroups.end()) {
			return nullptr;
		}
		return &(*it);
	}

	AudioGroup* AudioEngine::GetMasterAudioGroup()
	{
		return &s_audioGroups[MASTER_AUDIO_GROUP_INDEX];
	}


	Audio* AudioEngine::LoadAudioFromFile(const std::filesystem::path& filePath)
	{
		if (!std::filesystem::exists(filePath)) {
			nullptr;
		}
		auto it = std::find_if(s_audios.begin(), s_audios.end(), [name = filePath.string()](Audio* audio) {
			return name == audio->GetName();
			});
		if (it != s_audios.end()) {
			return (*it);
		}
		s_audios.push_back(new Audio(s_engine, filePath, s_audioGroups[MASTER_AUDIO_GROUP_INDEX]));
		return (*--s_audios.end());
	}

	Audio* AudioEngine::LoadAudioFromFile(const std::filesystem::path& filePath, const std::string_view& audioGroup)
	{
		if (!std::filesystem::exists(filePath)) {
			nullptr;
		}
		auto it = std::find_if(s_audios.begin(), s_audios.end(), [name = filePath.string()](Audio* audio) {
			return name == audio->GetName();
			});
		if (it != s_audios.end()) {
			return (*it);
		}
		auto _it = std::find_if(s_audioGroups.begin(), s_audioGroups.end(), [audioGroup](AudioGroup& group) {
			return audioGroup == group.GetName();
			});
		if (_it == s_audioGroups.end()) {
			s_audios.push_back(new Audio(s_engine, filePath, s_audioGroups[MASTER_AUDIO_GROUP_INDEX]));
		}
		s_audios.push_back(new Audio(s_engine, filePath, (*_it)));
		return (*--s_audios.end());
	}

	void AudioEngine::UnloadAudio(Audio* audio)
	{
		auto it = std::find_if(s_audios.begin(), s_audios.end(), [audio](Audio* _audio) {
			return audio == _audio;
			});
		if (it == s_audios.end()) {
			return;
		}
		s_audios.erase(it);
	}
}