#pragma once
#include "SongItem.g.h"
#include "ViewModel.SettingsViewModel.g.h"

namespace winrt::OsuPlayer::implementation
{
    struct SongItem : SongItemT<SongItem>
    {
        SongItem();

        SongItem(OsuPlayer::ViewModel::SongItemViewModel viewModel);

        OsuPlayer::ViewModel::SongItemViewModel ViewModel();

        //Methods
        void Grid_DoubleTapped(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Input::DoubleTappedRoutedEventArgs const& e);
        void CheckBox_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void CheckBox_Unchecked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
    private:
        OsuPlayer::ViewModel::SongItemViewModel m_model;
    public:
        winrt::Windows::Foundation::IAsyncAction PropertyMenuItem_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct SongItem : SongItemT<SongItem, implementation::SongItem>
    {
    };
}
