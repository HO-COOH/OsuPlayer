﻿#include "pch.h"
#include "ViewModel.MyMusic.h"
#if __has_include("ViewModel.MyMusicViewModel.g.cpp")
#include "ViewModel.MyMusicViewModel.g.cpp"
#endif

#include "Model.MyMusic.h"
#include "ViewModel.SettingsViewModel.g.h"
#include "SongItemDialog.g.h"
#include <winrt/Windows.ApplicationModel.Core.h>
#include <Generated Files/winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.Xaml.Media.Animation.h>
#include <ppltasks.h>
#include <pplawait.h>
#include "Utils.h"

namespace winrt::OsuPlayer::ViewModel::implementation
{
    MyMusicViewModel::MyMusicViewModel()
    {
        //After finished indexing, transform the song items to Views
        GetModel().onIndexingFinished(
            [this](std::vector<Model::SongItemModel> const& songs)
            {
                updateList();
            }
        );

        s_songItems.VectorChanged([](winrt::Windows::Foundation::Collections::IObservableVector<ViewModel::SongItemViewModel> v, winrt::Windows::Foundation::Collections::IVectorChangedEventArgs const& args)
        {
        });
    }

    int MyMusicViewModel::SortByIndex()
    {
        return static_cast<int>(GetModel().getSortOrder());
    }

    winrt::Windows::Foundation::IAsyncAction MyMusicViewModel::SortByIndex(int index)
    {
        co_await concurrency::create_task([index] {GetModel().setSortby(Model::MyMusicModel::SortByMethodIndex[index]); });
        updateList();
    }

    winrt::Windows::Foundation::Collections::IObservableVector<ViewModel::SongItemViewModel> MyMusicViewModel::Songs()
    {
        return s_songItems;
        
    }

    void implementation::MyMusicViewModel::Songs(winrt::Windows::Foundation::Collections::IObservableVector<ViewModel::SongItemViewModel> songs)
    {
        s_songItems = songs;
    }


    void implementation::MyMusicViewModel::updateList()
    {
        s_songItems.Clear();
        int i = 0;
        for (auto const& song : GetModel().m_songs)
        {
            SongItemViewModel viewModel;
            auto versions = viewModel.Versions();
            for (auto const& difficulty : song.Difficulties())
            {
                versions.Append(difficulty);
            }
            viewModel.ModelPointer(winrt::box_value<size_t>(reinterpret_cast<size_t>(&GetModel().m_songs[i++])));
            s_songItems.Append(viewModel);
        }
    }
}
