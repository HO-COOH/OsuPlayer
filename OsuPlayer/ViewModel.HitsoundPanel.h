#pragma once

#include "ViewModel.HitsoundPanelViewModel.g.h"
#include "ViewModel.HitsoundSampleSet.g.h"
#include "ViewModel.HitsoundSample.g.h"
#include <winrt/Windows.Media.Playback.h>

namespace winrt::OsuPlayer::ViewModel::implementation
{
    struct HitsoundSample : HitsoundSampleT<HitsoundSample>
    {
        HitsoundSample() = default;

        int Volume() { return m_volume; }
        void Volume(int volume) { m_volume = volume; }

        winrt::hstring Name() { return m_name; }
        void Name(winrt::hstring name) { m_name = name; }

        winrt::Windows::Foundation::IAsyncAction Play();
    private:
        static inline winrt::Windows::Media::Playback::MediaPlayer s_player;
        winrt::hstring m_name;
        int m_volume{};
    };


    struct HitsoundSampleSet : HitsoundSampleSetT<HitsoundSampleSet>
    {
        HitsoundSampleSet() = default;
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
    private:
        ViewModel::HitsoundSampleSet m_normal;
        ViewModel::HitsoundSampleSet m_soft;
        ViewModel::HitsoundSampleSet m_drum;
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
