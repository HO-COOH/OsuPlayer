#include "pch.h"
#include "OsuPathItemViewModel.h"
#if __has_include("OsuPathItemViewModel.g.cpp")
#include "OsuPathItemViewModel.g.cpp"
#endif
#include <winrt/Windows.UI.Xaml.Markup.h>

namespace winrt::OsuPlayer::implementation
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
