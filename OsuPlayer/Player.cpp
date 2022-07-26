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


    void Player::HalftimeCheckbox_Checked(
        winrt::Windows::Foundation::IInspectable const& sender, 
        winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        handlePlayMod(ViewModel::PlayMod::HalfTime);
    }


    void Player::DoubleTimeCheckbox_Checked(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        handlePlayMod(ViewModel::PlayMod::DoubleTime);
    }


    void Player::NightCoreCheckbox_Checked(
        winrt::Windows::Foundation::IInspectable const& sender, 
        winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        handlePlayMod(ViewModel::PlayMod::NightCore);
    }


    void Player::UseSkinHitsoundCheckbox_Checked(
        winrt::Windows::Foundation::IInspectable const& sender, 
        winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        ViewModel().UseSkinHitsound(true);
    }

    void Player::handlePlayMod(ViewModel::PlayMod newPlayMod)
    {
        if (newPlayMod == ViewModel().Mod())
            return;

        switch (newPlayMod)
        {
            case ViewModel::PlayMod::HalfTime:
                HalfTimeCheckbox().IsChecked(true);
                DoubleTimeCheckbox().IsChecked(false);
                NightCoreCheckbox().IsChecked(false);
                break;
            case ViewModel::PlayMod::DoubleTime:
                HalfTimeCheckbox().IsChecked(false);
                DoubleTimeCheckbox().IsChecked(true);
                NightCoreCheckbox().IsChecked(false);
                break;
            case ViewModel::PlayMod::NightCore:
                HalfTimeCheckbox().IsChecked(false);
                DoubleTimeCheckbox().IsChecked(false);
                NightCoreCheckbox().IsChecked(true);
                break;
            default:
                HalfTimeCheckbox().IsChecked(false);
                DoubleTimeCheckbox().IsChecked(false);
                NightCoreCheckbox().IsChecked(false);
                break;
        }
        ViewModel().Mod(newPlayMod);
    }


    void Player::HalfTimeCheckbox_Unchecked(
        winrt::Windows::Foundation::IInspectable const& sender, 
        winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        handlePlayMod(ViewModel::PlayMod::Normal);
    }


    void Player::DoubleTimeCheckbox_Unchecked(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        handlePlayMod(ViewModel::PlayMod::Normal);
    }


    void Player::NightCoreCheckbox_Unchecked(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        handlePlayMod(ViewModel::PlayMod::Normal);
    }

    void Player::UseSkinHitsoundCheckbox_Unchecked(
        winrt::Windows::Foundation::IInspectable const& sender, 
        winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        ViewModel().UseSkinHitsound(false);
    }

    OsuPlayer::ViewModel::PlayerViewModel Player::ViewModel()
    {
        return ViewModelLocator::Current().PlayerViewModel();
    }
}
