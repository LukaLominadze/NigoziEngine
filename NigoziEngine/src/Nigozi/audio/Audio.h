#pragma once
#include "miniaudio.h"
#include "AudioGroup.h"
#include <filesystem>

namespace Nigozi
{
	class Audio
	{
	public:
		Audio(ma_engine& engine, const std::filesystem::path& filePath, AudioGroup& audioGroup);
		~Audio();

		void SetVolume(float decibels);
		void SetAudioGroup(AudioGroup& audioGroup);

		void Play();
		void Pause();
		void Resume();
		void Stop();

		void Update();

		inline const bool IsPlaying() const { return m_isPlaying; }
		inline const bool IsPaused() const { return m_isPaused; }

		inline ma_sound& GetNativeAudio() { return m_audio; }
		inline const std::string_view GetAudioGroupName() { return m_audioGroupName; }
		inline const std::string& GetName() const { return m_name; }
		inline const std::filesystem::path GetFilePath() const { return m_filePath; }
	private:
		ma_sound m_audio;
		std::string_view m_audioGroupName;
		std::filesystem::path m_filePath;
		std::string m_name;

		bool m_isPlaying = false, m_isPaused = false;
	};
}

