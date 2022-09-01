#pragma once

#include "ViewModel.HitsoundPanelViewModel.g.h"
#include "ViewModel.HitsoundSampleSet.g.h"
#include "ViewModel.HitsoundSample.g.h"
#include <winrt/Windows.Media.Playback.h>
#include "Utils.PropertyChangeHelper.hpp"

namespace winrt::OsuPlayer::ViewModel::implementation
{
    struct HitsoundSample : HitsoundSampleT<HitsoundSample>, Utils::PropertyChangeHelper<HitsoundSample>
    {
        HitsoundSample(winrt::hstring name);

        int Volume() { return m_volume; }
        void Volume(int volume) { m_volume = volume; }

        winrt::hstring Name() { return m_name; }

        bool IsAvailable() { return m_hitsoundFile != nullptr; }
        
        void Play();
        void Save();
        winrt::Windows::Foundation::IAsyncAction Update();
        winrt::Windows::Foundation::IAsyncAction Replace();
        winrt::Windows::Media::Playback::IMediaPlaybackSource Source();
        winrt::Windows::Storage::StorageFile File() { return m_hitsoundFile; }
    private:
        static inline winrt::Windows::Media::Playback::MediaPlayer s_player;
        winrt::hstring m_name;
        winrt::Windows::Storage::StorageFile m_hitsoundFile{ nullptr };
        winrt::Windows::Media::Core::MediaSource m_source{ nullptr };
        int m_volume{};
    };


    struct HitsoundSampleSet : HitsoundSampleSetT<HitsoundSampleSet>
    {
        HitsoundSampleSet(winrt::hstring sampleSetName);
        ViewModel::HitsoundSample Hitnormal() { return m_hitNormal; }
        ViewModel::HitsoundSample Hitfinish() { return m_hitFinish; }
        ViewModel::HitsoundSample Hitwhistle() { return m_hitWhistle; }
        ViewModel::HitsoundSample Hitclap() { return m_hitClap; }
    private:
        ViewModel::HitsoundSample m_hitNormal;
        ViewModel::HitsoundSample m_hitFinish;
        ViewModel::HitsoundSample m_hitWhistle;
        ViewModel::HitsoundSample m_hitClap;
    };

    struct HitsoundPanelViewModel : HitsoundPanelViewModelT<HitsoundPanelViewModel>
    {
        HitsoundPanelViewModel() = default;

        ViewModel::HitsoundSampleSet Normal() { return m_normal; }
        ViewModel::HitsoundSampleSet Soft() { return m_soft; }
        ViewModel::HitsoundSampleSet Drum() { return m_drum; }

        winrt::Windows::Foundation::IAsyncAction Update();
    private:
        ViewModel::HitsoundSampleSet m_normal{ L"normal" };
        ViewModel::HitsoundSampleSet m_soft{ L"soft" };
        ViewModel::HitsoundSampleSet m_drum{ L"drum" };
    };
}

namespace winrt::OsuPlayer::ViewModel::factory_implementation
{
    struct HitsoundSample : HitsoundSampleT<HitsoundSample, implementation::HitsoundSample>
    {
    };

    struct HitsoundSampleSet : HitsoundSampleSetT<HitsoundSampleSet, implementation::HitsoundSampleSet>
    {
    };

    struct HitsoundPanelViewModel : HitsoundPanelViewModelT<HitsoundPanelViewModel, implementation::HitsoundPanelViewModel>
    {
    };
}
