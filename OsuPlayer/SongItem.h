#pragma once

#include "winrt/Windows.UI.Xaml.h"
#include "winrt/Windows.UI.Xaml.Markup.h"
#include "winrt/Windows.UI.Xaml.Interop.h"
#include "winrt/Windows.UI.Xaml.Controls.Primitives.h"
#include "winrt/Windows.Storage.h"

#include "SongItem.g.h"
#include "MyMusicModel.h"

namespace winrt::OsuPlayer::implementation
{
    struct SongItem : SongItemT<SongItem>, MyMusicItem
    {
        SongItem();
        SongItem(winrt::hstring songName);
        SongItem(winrt::hstring SongName, winrt::hstring Singer);
        SongItem(winrt::hstring SongName, winrt::hstring Singer, int Length);
        SongItem(winrt::hstring SongName, winrt::hstring Singer, int Length, winrt::hstring Mapper);

        winrt::hstring SongName() { return songName; }
        winrt::hstring Singer() { return singer; }
        winrt::hstring Mapper() { return mapper; }
        winrt::hstring Length();

        void SongName(winrt::hstring songName) { this->songName = songName; }
        void Singer(winrt::hstring singer) { this->singer = singer; }
        void Mapper(winrt::hstring mapper) { this->mapper = mapper; }

        int length{};

        void CheckBox_Checked(
            winrt::Windows::Foundation::IInspectable const& sender, 
            winrt::Windows::UI::Xaml::RoutedEventArgs const& e
        );

        void CheckBox_Unchecked(
            winrt::Windows::Foundation::IInspectable const& sender, 
            winrt::Windows::UI::Xaml::RoutedEventArgs const& e
        );
        void StackPanel_DoubleTapped(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Input::DoubleTappedRoutedEventArgs const& e);

        winrt::Windows::Storage::StorageFile SongFile() { return songFile; }
        void SongFile(winrt::Windows::Storage::StorageFile songFile) { this->songFile = songFile; }
    private:
        winrt::Windows::Storage::StorageFile songFile{nullptr};
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct SongItem : SongItemT<SongItem, implementation::SongItem>
    {
    };
}
