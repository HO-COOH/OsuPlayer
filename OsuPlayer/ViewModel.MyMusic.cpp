#include "pch.h"
#include "ViewModel.MyMusic.h"
#if __has_include("ViewModel.MyMusicViewModel.g.cpp")
#include "ViewModel.MyMusicViewModel.g.cpp"
#endif

#include "Model.MyMusic.h"
#include "ViewModel.SettingsViewModel.g.h"
#include <winrt/Windows.ApplicationModel.Core.h>
#include <Generated Files/winrt/Windows.UI.Core.h>

namespace winrt::OsuPlayer::ViewModel::implementation
{
    MyMusicViewModel::MyMusicViewModel()
    {
        Model::MyMusicModel::OnIndexingFinished(
            [this](std::vector<Model::SongItemModel> const& songs)
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
                    auto versions = viewModel.Versions();
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

    int MyMusicViewModel::SortByIndex()
    {
        return static_cast<int>(Model::MyMusicModel::m_sortOrder);
    }

    void MyMusicViewModel::SortByIndex(int index)
    {
        Model::MyMusicModel::m_sortBy = Model::MyMusicModel::SortByMethodIndex[index];
    }

    winrt::Windows::Foundation::Collections::IObservableVector<OsuPlayer::SongItem> MyMusicViewModel::Songs()
    {
        return s_songItems;
        
    }

    winrt::Windows::Foundation::IAsyncAction MyMusicViewModel::ShowPropertyOf(int index, int versionIndex)
    {
        auto const& songItem = Model::MyMusicModel::get(index);

        
       
        OsuPlayer::SongItemDialog content;
        OsuPlayer::ViewModel::SongPropertyViewModel model;


        model.Tags(winrt::to_hstring(songItem.Tags(versionIndex)));
        model.Bitrate(winrt::to_hstring(songItem.BitRate()) + L" kbps");
        model.SongPath(songItem.VersionFiles()[versionIndex].Path());
        model.Title(songItem.SongName());


        winrt::Windows::UI::Xaml::Controls::ContentDialog propertyDialog;
        
        content.ViewModel(model);
       
        propertyDialog.Content(content);
        propertyDialog.CloseButtonText(L"Close");
        co_await propertyDialog.ShowAsync();
    }
}
