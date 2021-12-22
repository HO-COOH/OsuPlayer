﻿#include "pch.h"
#include "MyMusicViewModel.h"
#if __has_include("MyMusicViewModel.g.cpp")
#include "MyMusicViewModel.g.cpp"
#endif

#include "MyMusicModel.h"
#include "SongItemViewModel.g.h"

namespace winrt::OsuPlayer::implementation
{
    MyMusicViewModel::MyMusicViewModel()
    {
        MyMusicModel::OnIndexingFinished(
            [this](std::vector<SongItemModel> const& songs)
            {
                s_songItems.Clear();
                int i = 0;
                for (auto const& song : songs)
                {
                    SongItemViewModel viewModel;
                    viewModel.SongName(song.SongName());
                    viewModel.Singer(song.Singer());
                    viewModel.Length(song.Length());
                    viewModel.Mapper(song.Mapper());
                    auto& versions = viewModel.Versions();
                    for (auto const& path : song.VersionFiles())
                    {
                        versions.Append(path.Path());
                    }
                    viewModel.Index(i++);
                    s_songItems.Append(SongItem{ viewModel });
                }
                
            }
        );
    }
    OsuPlayer::SortBy MyMusicViewModel::SortBy()
    {
        return OsuPlayer::SortBy();
    }
    void MyMusicViewModel::SortBy(OsuPlayer::SortBy SortMethod)
    {
    }

    winrt::Windows::Foundation::Collections::IObservableVector<SongItem> MyMusicViewModel::Songs()
    {
        return s_songItems;
        
    }
}
