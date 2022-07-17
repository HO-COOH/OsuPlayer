#include "pch.h"
#include "ViewModel.SongProperty.h"
#if __has_include("ViewModel.SongPropertyViewModel.g.cpp")
#include "ViewModel.SongPropertyViewModel.g.cpp"
#endif

namespace winrt::OsuPlayer::ViewModel::implementation
{
    winrt::hstring SongPropertyViewModel::Title()
    {
        return m_title;
    }

    void SongPropertyViewModel::Title(winrt::hstring title)
    {
        m_title = title;
    }

    winrt::hstring SongPropertyViewModel::Tags()
    {
        return m_tags;
    }

    void SongPropertyViewModel::Tags(winrt::hstring tags)
    {
        m_tags = tags;
    }

    winrt::hstring SongPropertyViewModel::Bitrate()
    {
        return m_bitrate;
    }

    void SongPropertyViewModel::Bitrate(winrt::hstring bitrate)
    {
        m_bitrate = bitrate;
    }

    winrt::hstring SongPropertyViewModel::Length()
    {
        return m_length;
    }

    void SongPropertyViewModel::Length(winrt::hstring length)
    {
        m_length = length;
    }

    winrt::hstring SongPropertyViewModel::SongPath()
    {
        return m_songPath;
    }

    void SongPropertyViewModel::SongPath(winrt::hstring songPath)
    {
        m_songPath = songPath;
    }
}
