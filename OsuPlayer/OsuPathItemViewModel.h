#pragma once

#include "OsuPathItemViewModel.g.h"
#include <winrt/Windows.UI.Xaml.Markup.h>

namespace winrt::OsuPlayer::implementation
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

namespace winrt::OsuPlayer::factory_implementation
{
    struct OsuPathItemViewModel : OsuPathItemViewModelT<OsuPathItemViewModel, implementation::OsuPathItemViewModel>
    {
    };
}
