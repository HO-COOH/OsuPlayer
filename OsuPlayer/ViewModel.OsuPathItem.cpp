#include "pch.h"
#include "ViewModel.OsuPathItem.h"
#if __has_include("ViewModel.OsuPathItemViewModel.g.cpp")
#include "ViewModel.OsuPathItemViewModel.g.cpp"
#endif
#include <winrt/Windows.UI.Xaml.Markup.h>

namespace winrt::OsuPlayer::ViewModel::implementation
{
    bool OsuPathItemViewModel::IsLoading()
    {
        return m_isLoading;
    }

    void OsuPathItemViewModel::IsLoading(bool isLoading)
    {
        m_isLoading = isLoading;
    }

    winrt::hstring OsuPathItemViewModel::Path()
    {
        return m_path;
    }

    void OsuPathItemViewModel::Path(winrt::hstring path)
    {
        m_path = path;
    }

    int OsuPathItemViewModel::BeatmapCount()
    {
        return m_beatmapCount;
    }

    void OsuPathItemViewModel::BeatmapCount(int count)
    {
        m_beatmapCount = count;
    }
}
