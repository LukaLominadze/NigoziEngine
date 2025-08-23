#pragma once

#include "miniaudio.h"

namespace Nigozi
{
	class AudioGroup
	{
	public:
		AudioGroup(ma_engine& engine, const std::string& name);
		AudioGroup(ma_engine& engine, const std::string& name, AudioGroup& parent);
		~AudioGroup();

		void SetVolume(float decibels);

		ma_sound_group& GetNativeAudioGroup() { return m_audioGroup; }
		const std::string& GetName() const { return m_name; }
	private:
		ma_sound_group m_audioGroup;
		std::string m_name;
	};
}
