#pragma once

#include "ViewModel.MyMusicViewModel.g.h"
#include "Utils.PropertyChangeHelper.hpp"
#include "Model.MyMusic.h"
#include "ViewModel.SongItemViewModel.g.h"

namespace winrt::OsuPlayer::ViewModel::implementation
{
    struct MyMusicViewModel : MyMusicViewModelT<MyMusicViewModel>, Utils::PropertyChangeHelper<MyMusicViewModel>
    {
        MyMusicViewModel();
        
        int SortByIndex();
        winrt::Windows::Foundation::IAsyncAction SortByIndex(int index);

        winrt::Windows::Foundation::Collections::IObservableVector<ViewModel::SongItemViewModel> Songs();
        void Songs(winrt::Windows::Foundation::Collections::IObservableVector<ViewModel::SongItemViewModel> songs);

        winrt::Windows::Foundation::IAsyncAction ShowPropertyOf(ViewModel::SongItemViewModel songItem);
    private:
        void updateList();
        inline static winrt::Windows::Foundation::Collections::IObservableVector<ViewModel::SongItemViewModel> s_songItems = winrt::single_threaded_observable_vector<ViewModel::SongItemViewModel>();
        static Model::MyMusicModel& GetModel() { return Model::MyMusicModel().GetInstance(); }
    };
}

namespace winrt::OsuPlayer::ViewModel::factory_implementation
{
    struct MyMusicViewModel : MyMusicViewModelT<MyMusicViewModel, implementation::MyMusicViewModel>
    {
    };
}
