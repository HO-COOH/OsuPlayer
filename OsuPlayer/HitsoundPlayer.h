#pragma once

#include <winrt/Windows.Media.Audio.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Storage.h>

struct HitObject;
struct TimingPoint;

class HitsoundPlayer
{
	winrt::Windows::Media::Audio::AudioGraph m_audioGraph{ nullptr };
	winrt::Windows::Media::Audio::AudioDeviceOutputNode m_outputNode{ nullptr };
	bool m_hasInit = false;
	bool m_hasStart = false;
	double m_volume{};

	winrt::Windows::Storage::StorageFolder m_beatmapFolder{ nullptr };

	/**
	 * @brief 
	 * @param hitObject 
	 * @param isNormalSampleSet true for NormalSet, false for AdditionSet
	 * @param timingPoint 
	 * @return 
	*/
	winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Storage::StorageFile> getHitsoundFile(
		HitObject const& hitObject, 
		bool isNormalSampleSet = true,
		TimingPoint const* timingPoint = nullptr
	);
	
	/**
	 * @brief This volume is not normalized, ranging 1~100
	*/
	int getHitsoundVolume(HitObject const& hitObject, TimingPoint const* timingPoint = nullptr);

	void addInputNode(winrt::Windows::Media::Audio::AudioFileInputNode node);
	winrt::Windows::Foundation::IAsyncAction addInputNode(winrt::Windows::Storage::StorageFile file);
public:
	winrt::Windows::Foundation::IAsyncAction init();
	bool hasInit() const { return m_hasInit; }
	void playHitsound(HitObject const& hitObject, TimingPoint const* timingPoint = nullptr);

	void setFolder(winrt::Windows::Storage::StorageFolder folder) { m_beatmapFolder = folder; }

	/**
	 * @brief Return a volume ranging 0.0 ~ 1.0
	*/
	double NormalizedVolume();

	/**
	 * @brief Setting a normalized volume
	 * @param volume Should range from 0.0 ~ 1.0
	*/
	void NormalizedVolume(double volume);
};

