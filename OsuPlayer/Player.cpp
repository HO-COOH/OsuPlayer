#include "pch.h"
#include "Player.h"
#if __has_include("Player.g.cpp")
#include "Player.g.cpp"
#endif
#include "ViewModelLocator.h"


using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::OsuPlayer::implementation
{
    Player::Player()
    {
        InitializeComponent();
    }

    void Player::handlePlayMod(ViewModel::PlayMod newPlayMod)
    {
        ViewModel().Mod(newPlayMod);
    }

    OsuPlayer::ViewModel::PlayerViewModel Player::ViewModel()
    {
        return ViewModelLocator::Current().PlayerViewModel();
    }
    OsuPlayer::ViewModel::SettingsViewModel Player::SettingsViewModel()
    {
        return ViewModelLocator::Current().SettingsViewModel();
    }

    void Player::HalfTimeItem_Click(
        [[maybe_unused]]winrt::Windows::Foundation::IInspectable const& sender, 
        [[maybe_unused]]winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        handlePlayMod(ViewModel::PlayMod::HalfTime);
    }


    void Player::DoubleTimeItem_Click(
        [[maybe_unused]]winrt::Windows::Foundation::IInspectable const& sender, 
        [[maybe_unused]]winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        handlePlayMod(ViewModel::PlayMod::DoubleTime);
    }

    void Player::NightCoreItem_Click(
        [[maybe_unused]]winrt::Windows::Foundation::IInspectable const& sender, 
        [[maybe_unused]]winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        handlePlayMod(ViewModel::PlayMod::NightCore);
    }


    void winrt::OsuPlayer::implementation::Player::UseSkinHitsoundCheckbox_Checked(
        [[maybe_unused]]winrt::Windows::Foundation::IInspectable const& sender, 
        [[maybe_unused]]winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {

    }


    void winrt::OsuPlayer::implementation::Player::UseSkinHitsoundCheckbox_Unchecked(
        [[maybe_unused]]winrt::Windows::Foundation::IInspectable const& sender, 
        [[maybe_unused]]winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {

    }

}
