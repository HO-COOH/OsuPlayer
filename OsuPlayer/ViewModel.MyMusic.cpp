#include "pch.h"
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
#include <winrt/Windows.Media.Playback.h>
#include <ppltasks.h>
#include <pplawait.h>
#include "Utils.h"
#include "ViewModelLocator.h"

namespace winrt::OsuPlayer::ViewModel::implementation
{
    MyMusicViewModel::MyMusicViewModel()
    {
        //After finished indexing, transform the song items to Views
        GetModel().onIndexingFinished(
            [this](std::vector<Model::SongItemModel> const&)
            {
                updateList();
                updateCollection();
            }
        );
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

    void MyMusicViewModel::Songs(winrt::Windows::Foundation::Collections::IObservableVector<ViewModel::SongItemViewModel> songs)
    {
        s_songItems = songs;
    }

    winrt::Windows::Foundation::IAsyncAction MyMusicViewModel::PlayCollection()
    {
        winrt::Windows::Media::Playback::MediaPlaybackList list;
        for (auto item : s_songItems)
        {
            list.Items().Append(
                winrt::Windows::Media::Playback::MediaPlaybackItem{ reinterpret_cast<Model::SongItemModel*>(winrt::unbox_value<size_t>(item.ModelPointer()))->Source() }
            );
        }
        co_await ViewModelLocator::Current().PlayerViewModel().PlayList(list);
    }

    void MyMusicViewModel::updateList()
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
    void MyMusicViewModel::updateCollection()
    {
        auto collections = ViewModelLocator::Current().Collections();
        collections.Clear();
        for (auto& collection : GetModel().m_collections)
        {
            CollectionItem item;
            item.ModelPointer(winrt::box_value(reinterpret_cast<size_t>(&collection)));
            collections.Append(item);
            ViewModelLocator::Current().updateCollectionItem(item);
        }
    }
}
