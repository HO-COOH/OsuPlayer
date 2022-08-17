#pragma once

#include "ViewModel.MyMusicViewModel.g.h"
#include "Utils.PropertyChangeHelper.hpp"
#include "Model.MyMusic.h"
#include "ViewModel.SongItemViewModel.g.h"
#include "ViewModel.CollectionItem.g.h"

namespace winrt::OsuPlayer::ViewModel::implementation
{
    struct MyMusicViewModel : MyMusicViewModelT<MyMusicViewModel>, Utils::PropertyChangeHelper<MyMusicViewModel>
    {
        MyMusicViewModel();
        
        int SortByIndex();
        winrt::Windows::Foundation::IAsyncAction SortByIndex(int index);

        winrt::hstring ListName() { return m_name; }
        void ListName(winrt::hstring name) { m_name = name; }

        winrt::Windows::Foundation::Collections::IObservableVector<ViewModel::SongItemViewModel> Songs();
        void Songs(winrt::Windows::Foundation::Collections::IObservableVector<ViewModel::SongItemViewModel> songs);
    private:
        void updateList();
        void updateCollection();
        winrt::hstring m_name;
        winrt::Windows::Foundation::Collections::IObservableVector<ViewModel::SongItemViewModel> s_songItems = winrt::single_threaded_observable_vector<ViewModel::SongItemViewModel>();
        static Model::MyMusicModel& GetModel() { return Model::MyMusicModel().GetInstance(); }
    };
}

namespace winrt::OsuPlayer::ViewModel::factory_implementation
{
    struct MyMusicViewModel : MyMusicViewModelT<MyMusicViewModel, implementation::MyMusicViewModel>
    {
    };
}
