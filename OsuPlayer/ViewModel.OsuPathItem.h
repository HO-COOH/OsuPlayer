#pragma once

#include "ViewModel.OsuPathItemViewModel.g.h"
#include <winrt/Windows.UI.Xaml.Markup.h>

namespace winrt::OsuPlayer::ViewModel::implementation
{
    struct OsuPathItemViewModel : OsuPathItemViewModelT<OsuPathItemViewModel>
    {
        OsuPathItemViewModel() = default;
        
        //Properties
        bool IsLoading();
        void IsLoading(bool isLoading);
        
        winrt::hstring Path();
        void Path(winrt::hstring path);

        int BeatmapCount();
        void BeatmapCount(int count);
    private:
        bool m_isLoading{};
        winrt::hstring m_path;
        int m_beatmapCount{};
    };
}

namespace winrt::OsuPlayer::ViewModel::factory_implementation
{
    struct OsuPathItemViewModel : OsuPathItemViewModelT<OsuPathItemViewModel, implementation::OsuPathItemViewModel>
    {
    };
}
