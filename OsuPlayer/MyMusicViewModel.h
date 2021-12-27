#pragma once

#include "MyMusicViewModel.g.h"

namespace winrt::OsuPlayer::implementation
{
    struct MyMusicViewModel : MyMusicViewModelT<MyMusicViewModel>
    {
        MyMusicViewModel();
        
        int SortByIndex();

        void SortByIndex(int index);

        winrt::Windows::Foundation::Collections::IObservableVector<SongItem> Songs();
    private:
        inline static winrt::Windows::Foundation::Collections::IObservableVector<SongItem> s_songItems = winrt::single_threaded_observable_vector<SongItem>();
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct MyMusicViewModel : MyMusicViewModelT<MyMusicViewModel, implementation::MyMusicViewModel>
    {
    };
}
