#pragma once

#include "Player.g.h"
#include "ViewModel.PlayerViewModel.g.h"
#include "ViewModel.SettingsViewModel.g.h"

/*
    According to microsoft: https://docs.microsoft.com/zh-cn/uwp/api/windows.ui.xaml.media.imagesource?view=winrt-22000

*/
namespace winrt::OsuPlayer::implementation
{
    struct Player : PlayerT<Player>
    {
        Player();
        
        OsuPlayer::ViewModel::PlayerViewModel ViewModel();
        OsuPlayer::ViewModel::SettingsViewModel SettingsViewModel();
    private:
        void handlePlayMod(ViewModel::PlayMod newPlayMod);

        bool m_showingSimpleVolume = true;
    public:
        void HalfTimeItem_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void DoubleTimeItem_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void NightCoreItem_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void UseSkinHitsoundCheckbox_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void UseSkinHitsoundCheckbox_Unchecked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct Player : PlayerT<Player, implementation::Player>
    {
    };
}
