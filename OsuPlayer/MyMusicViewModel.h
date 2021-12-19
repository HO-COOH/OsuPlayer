#pragma once

#include "MyMusicViewModel.g.h"

namespace winrt::OsuPlayer::implementation
{
    struct MyMusicViewModel : MyMusicViewModelT<MyMusicViewModel>
    {
        MyMusicViewModel() = default;
        
        OsuPlayer::SortBy SortBy();

        void SortBy(OsuPlayer::SortBy SortMethod);

        winrt::Windows::Foundation::Collections::IObservableVector<SongItem> Songs();
    private:

    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct MyMusicViewModel : MyMusicViewModelT<MyMusicViewModel, implementation::MyMusicViewModel>
    {
    };
}
