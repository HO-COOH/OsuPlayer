#pragma once

#include "ViewModel.MyMusicViewModel.g.h"

namespace winrt::OsuPlayer::ViewModel::implementation
{
    struct MyMusicViewModel : MyMusicViewModelT<MyMusicViewModel>
    {
        MyMusicViewModel();
        
        int SortByIndex();

        void SortByIndex(int index);

        winrt::Windows::Foundation::Collections::IObservableVector<OsuPlayer::SongItem> Songs();

        winrt::Windows::Foundation::IAsyncAction ShowPropertyOf(int index, int versionIndex);
    private:
        inline static winrt::Windows::Foundation::Collections::IObservableVector<OsuPlayer::SongItem> s_songItems = winrt::single_threaded_observable_vector<OsuPlayer::SongItem>();
    };
}

namespace winrt::OsuPlayer::ViewModel::factory_implementation
{
    struct MyMusicViewModel : MyMusicViewModelT<MyMusicViewModel, implementation::MyMusicViewModel>
    {
    };
}
