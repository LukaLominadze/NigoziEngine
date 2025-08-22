#include "ngpch.h"
#include "Audio.h"

namespace Nigozi
{
	Audio::Audio(ma_engine& engine, const std::filesystem::path& filePath, AudioGroup& audioGroup)
		:m_audioGroupName(audioGroup.GetName()), m_name(filePath.filename().string())
	{
		ma_result result = ma_sound_init_from_file(&engine, filePath.string().c_str(), NULL, &audioGroup.GetNativeAudioGroup(), NULL, &m_audio);
		if (result != MA_SUCCESS) {
			LOG("Couldn't load file!");
		}
	}

	Audio::~Audio()
	{
		ma_sound_uninit(&m_audio);
	}

	void Audio::SetVolume(float decibels)
	{
		ma_sound_set_volume(&m_audio, ma_volume_db_to_linear(decibels));
	}

	void Audio::SetAudioGroup(const std::string_view& name)
	{
	}

	void Audio::Play()
	{
		ma_sound_seek_to_pcm_frame(&m_audio, 0);
		ma_result result = ma_sound_start(&m_audio);
		if (result != MA_SUCCESS) {
			LOG("Can't play sound!");
		}
		m_isPlaying = true;
		m_isPaused = false;
	}

	void Audio::Pause()
	{
		if (!m_isPlaying || m_isPaused) {
			return;
		}
		ma_result result = ma_sound_stop(&m_audio);
		if (result != MA_SUCCESS) {
			LOG("Can't play sound!");
		}
		m_isPaused = true;
	}

	void Audio::Resume()
	{
		if (!m_isPlaying) {
			return;
		}
		ma_result result = ma_sound_start(&m_audio);
		if (result != MA_SUCCESS) {
			LOG("Can't play sound!");
		}
		m_isPaused = false;
	}

	void Audio::Stop()
	{
		ma_result result = ma_sound_stop(&m_audio);
		if (result != MA_SUCCESS) {
			LOG("Can't play sound!");
		}
		m_isPlaying = false;
		m_isPaused = false;
	}

	void Audio::Update()
	{
		if (ma_sound_at_end(&m_audio)) {
			m_isPlaying = false;
			m_isPaused = false;
		}
	}
}