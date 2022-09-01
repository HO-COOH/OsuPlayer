#pragma once

#include <winrt/Windows.Media.Audio.h>
#include <winrt/Windows.Foundation.h>

class HitObject;

class HitsoundPlayer
{
	winrt::Windows::Media::Audio::AudioGraph m_audioGraph{ nullptr };
	winrt::Windows::Media::Audio::AudioDeviceOutputNode m_outputNode{ nullptr };
	bool m_hasInit = false;
	bool m_hasStart = false;
	int m_volume{};
public:
	winrt::Windows::Foundation::IAsyncAction init();
	bool hasInit() const { return m_hasInit; }
	winrt::Windows::Foundation::IAsyncAction playHitsound(HitObject const& hitObject);
	int Volume();
	void Volume(int volume);
};

