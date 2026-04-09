#include "ngpch.h"
#include "AudioGroup.h"
#include "core/Log.h"

namespace Nigozi
{
	AudioGroup::AudioGroup(ma_engine& engine, const std::string& name)
		:m_name(name)
	{
		ma_result result = ma_sound_group_init(&engine, NULL, NULL, &m_audioGroup);
		if (result != MA_SUCCESS) {
			NG_CORE_LOG_ERROR("[Audio] Couldn't initialize audio group!");
		}
	}

	AudioGroup::AudioGroup(ma_engine& engine, const std::string& name, AudioGroup& parent)
		:m_name(name)
	{
		ma_result result = ma_sound_group_init(&engine, NULL, &parent.GetNativeAudioGroup(), &m_audioGroup);
		if (result != MA_SUCCESS) {
			NG_CORE_LOG_ERROR("[Audio] Couldn't initialize audio group!");
		}
	}

	AudioGroup::~AudioGroup()
	{
		ma_sound_group_uninit(&m_audioGroup);
	}

	void AudioGroup::SetVolume(float decibels)
	{
		ma_sound_group_set_volume(&m_audioGroup, ma_volume_db_to_linear(decibels));
	}
}