#pragma once

#include "Player.g.h"
#include "ViewModel.PlayerViewModel.g.h"

/*
    According to microsoft: https://docs.microsoft.com/zh-cn/uwp/api/windows.ui.xaml.media.imagesource?view=winrt-22000

*/
namespace winrt::OsuPlayer::implementation
{
    struct Player : PlayerT<Player>
    {
        Player();

        void PlayButton_Click(
            winrt::Windows::Foundation::IInspectable const& sender, 
            winrt::Windows::UI::Xaml::RoutedEventArgs const& e);

        void NextSongButton_Click(
            winrt::Windows::Foundation::IInspectable const& sender, 
            winrt::Windows::UI::Xaml::RoutedEventArgs const& e);

        void PreviousSongButton_Click(
            winrt::Windows::Foundation::IInspectable const& sender, 
            winrt::Windows::UI::Xaml::RoutedEventArgs const& e);

        void HalftimeCheckbox_Checked(
            winrt::Windows::Foundation::IInspectable const& sender, 
            winrt::Windows::UI::Xaml::RoutedEventArgs const& e);

        void DoubleTimeCheckbox_Checked(
            winrt::Windows::Foundation::IInspectable const& sender, 
            winrt::Windows::UI::Xaml::RoutedEventArgs const& e);

        void NightCoreCheckbox_Checked(
            winrt::Windows::Foundation::IInspectable const& sender, 
            winrt::Windows::UI::Xaml::RoutedEventArgs const& e);

        void UseSkinHitsoundCheckbox_Checked(
            winrt::Windows::Foundation::IInspectable const& sender, 
            winrt::Windows::UI::Xaml::RoutedEventArgs const& e);

        void MuteButton_Click(
            winrt::Microsoft::UI::Xaml::Controls::SplitButton const& sender, 
            winrt::Microsoft::UI::Xaml::Controls::SplitButtonClickEventArgs const& args);

        void HalfTimeCheckbox_Unchecked(
            winrt::Windows::Foundation::IInspectable const& sender, 
            winrt::Windows::UI::Xaml::RoutedEventArgs const& e);

        void DoubleTimeCheckbox_Unchecked(
            winrt::Windows::Foundation::IInspectable const& sender, 
            winrt::Windows::UI::Xaml::RoutedEventArgs const& e);

        void NightCoreCheckbox_Unchecked(
            winrt::Windows::Foundation::IInspectable const& sender, 
            winrt::Windows::UI::Xaml::RoutedEventArgs const& e);

        void UseSkinHitsoundCheckbox_Unchecked(
            winrt::Windows::Foundation::IInspectable const& sender, 
            winrt::Windows::UI::Xaml::RoutedEventArgs const& e);

        void VolumeSlider_ValueChanged(
            winrt::Windows::Foundation::IInspectable const& sender, 
            winrt::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs const& e);
        
        OsuPlayer::ViewModel::PlayerViewModel ViewModel();
    private:
        void handlePlayMod(ViewModel::PlayMod newPlayMod);
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct Player : PlayerT<Player, implementation::Player>
    {
    };
}
