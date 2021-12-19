#include "pch.h"
#include "MyMusicViewModel.h"
#if __has_include("MyMusicViewModel.g.cpp")
#include "MyMusicViewModel.g.cpp"
#endif

namespace winrt::OsuPlayer::implementation
{
    OsuPlayer::SortBy MyMusicViewModel::SortBy()
    {
        return OsuPlayer::SortBy();
    }
    void MyMusicViewModel::SortBy(OsuPlayer::SortBy SortMethod)
    {
    }
    winrt::Windows::Foundation::Collections::IObservableVector<SongItem> MyMusicViewModel::Songs()
    {
        return winrt::Windows::Foundation::Collections::IObservableVector<SongItem>();
    }
}
