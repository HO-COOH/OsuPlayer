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

    winrt::Windows::Foundation::IAsyncAction implementation::MyMusicViewModel::ShowPropertyOf(ViewModel::SongItemViewModel songItemViewModel)
    {
        auto& songItem = *reinterpret_cast<Model::SongItemModel*>(winrt::unbox_value<size_t>(songItemViewModel.ModelPointer()));
        int versionIndex = songItemViewModel.SelectedVersionIndex();


        OsuPlayer::SongItemDialog content;

        auto tagTask = concurrency::create_task([&songItem, versionIndex] {return songItem.Tags(versionIndex); });
        auto bitrateTask = concurrency::create_task([&songItem] {return songItem.BitRate(); });

        content.Tags(winrt::to_hstring(co_await tagTask));
        content.Bitrate(winrt::to_hstring(co_await bitrateTask) + L" kbps");
        content.SongPath(songItem.Difficulties()[versionIndex]);
        content.Title(songItem.SongName());
        content.Singer(songItem.Singer());
        content.Length(Utils::GetDurationString(Utils::GetDuration(songItem.Length())));


        winrt::Windows::UI::Xaml::Controls::ContentDialog propertyDialog;
        propertyDialog.Content(content);
        propertyDialog.CloseButtonText(L"Close");
        //Add some animations
        winrt::Windows::UI::Xaml::Media::Animation::TransitionCollection transitions;
        transitions.Append(winrt::Windows::UI::Xaml::Media::Animation::EntranceThemeTransition{});
        propertyDialog.Transitions(transitions);
        co_await propertyDialog.ShowAsync();
    }

    void implementation::MyMusicViewModel::updateList()
    {
        s_songItems.Clear();
        int i = 0;
        for (auto const& song : GetModel().m_songs)
        {
            SongItemViewModel viewModel;
            viewModel.SongName(song.SongName());
            viewModel.Singer(song.Singer());
            viewModel.Length(song.Length());
            viewModel.Mapper(song.Mapper());
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
