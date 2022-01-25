#include "pch.h"
#include "MyMusicViewModel.h"
#if __has_include("MyMusicViewModel.g.cpp")
#include "MyMusicViewModel.g.cpp"
#endif

#include "MyMusicModel.h"
#include "SongItemViewModel.g.h"
#include <winrt/Windows.ApplicationModel.Core.h>
#include <Generated Files/winrt/Windows.UI.Core.h>

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
        return static_cast<int>(MyMusicModel::m_sortOrder);
    }

    void MyMusicViewModel::SortByIndex(int index)
    {
        MyMusicModel::m_sortBy = MyMusicModel::SortByMethodIndex[index];
    }

    winrt::Windows::Foundation::Collections::IObservableVector<SongItem> MyMusicViewModel::Songs()
    {
        return s_songItems;
        
    }

    winrt::Windows::Foundation::IAsyncAction MyMusicViewModel::ShowPropertyOf(int index, int versionIndex)
    {
        auto const& songItem = MyMusicModel::get(index);

        
       
        OsuPlayer::SongItemDialog content;
        OsuPlayer::SongPropertyViewModel model;

        auto future = std::async(std::launch::async, [&]() {return songItem.Tags(versionIndex); });
        auto result = co_await future;
        model.Tags(winrt::to_hstring(result));
        model.Bitrate(winrt::to_hstring(songItem.BitRate()) + L" kbps");
        model.SongPath(songItem.VersionFiles()[versionIndex].Path());
        model.Title(songItem.SongName());


        co_await winrt::resume_foreground(winrt::Windows::ApplicationModel::Core::CoreApplication::MainView().CoreWindow().Dispatcher());
        winrt::Windows::UI::Xaml::Controls::ContentDialog propertyDialog;
        
        content.ViewModel(model);
       
        propertyDialog.Content(content);
        propertyDialog.CloseButtonText(L"Close");
        co_await propertyDialog.ShowAsync();
    }
}
