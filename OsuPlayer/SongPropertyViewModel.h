#pragma once

#include "SongPropertyViewModel.g.h"

namespace winrt::OsuPlayer::implementation
{
    struct SongPropertyViewModel : SongPropertyViewModelT<SongPropertyViewModel>
    {
        SongPropertyViewModel() = default;

        winrt::hstring Title();
        void Title(winrt::hstring title);

        winrt::hstring Tags();
        void Tags(winrt::hstring tags);

        winrt::hstring Bitrate();
        void Bitrate(winrt::hstring bitrate);

        winrt::hstring Length();
        void Length(winrt::hstring length);

        winrt::hstring SongPath();
        void SongPath(winrt::hstring songPath);
    private:
        winrt::hstring m_title;
        winrt::hstring m_tags;
        winrt::hstring m_bitrate;
        winrt::hstring m_length;
        winrt::hstring m_songPath;
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct SongPropertyViewModel : SongPropertyViewModelT<SongPropertyViewModel, implementation::SongPropertyViewModel>
    {
    };
}
