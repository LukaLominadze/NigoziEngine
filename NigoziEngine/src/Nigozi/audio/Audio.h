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
		void SetAudioGroup(const std::string_view& name);

		void Play();
		void Pause();
		void Resume();
		void Stop();

		void Update();

		inline const bool IsPlaying() const { return m_isPlaying; }
		inline const bool IsPaused() const { return m_isPaused; }

		inline ma_sound& GetNativeAudio() { return m_audio; }
		inline const std::string_view& GetAudioGroupName() { return m_audioGroupName; }
		inline const std::string& GetName() const { return m_name; }
	private:
		ma_sound m_audio;
		std::string_view m_audioGroupName;
		std::string m_name;

		bool m_isPlaying = false, m_isPaused = false;
	};
}

