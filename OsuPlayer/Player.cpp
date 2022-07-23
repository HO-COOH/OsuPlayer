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

    void Player::PlayButton_Click(
        winrt::Windows::Foundation::IInspectable const& sender, 
        winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        //if (isPlaying)
        //{
        //    /*Should pause, Render a play icon*/
        //    PlayButtonIcon().Symbol(winrt::Windows::UI::Xaml::Controls::Symbol::Play);
        //    songPlayer.Pause();
        //}
        //else
        //{
        //    /*Should play, render a pause icon*/
        //    PlayButtonIcon().Symbol(winrt::Windows::UI::Xaml::Controls::Symbol::Pause);
        //    co_await songPlayer.Play(LR"(D:\osu\Songs\609057 love solfege - Vanity Clock\love solfege - Vanity Clock.mp3)");
        //}
        //isPlaying = !isPlaying;
        ViewModel().Play();
    }


    void Player::NextSongButton_Click(
        winrt::Windows::Foundation::IInspectable const& sender, 
        winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        ViewModel().PlayNext();
    }


    void Player::PreviousSongButton_Click(
        winrt::Windows::Foundation::IInspectable const& sender, 
        winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        ViewModel().PlayPrevious();
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

    void Player::MuteButton_Click(
        winrt::Microsoft::UI::Xaml::Controls::SplitButton const& sender, 
        winrt::Microsoft::UI::Xaml::Controls::SplitButtonClickEventArgs const& args)
    {
        ViewModel().Volume(0);
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

    //void Player::Play(winrt::Windows::Storage::StorageFile song)
    //{
    //    songPlayer.Play(song);
    //    auto const length = songPlayer.getLength();
    //    SongLengthText().Text(Utils::GetDurationString(Utils::GetDuration(length)));

    //    PlayingSlider().Maximum(length);
    //    songPlayer.player.PlaybackSession().PositionChanged(
    //        [this](auto session, auto const& _)->winrt::Windows::Foundation::IAsyncAction
    //        {
    //            //auto const percentage = (static_cast<long double>(session.Position().count()) / session.NaturalDuration().count()) * 100.0;
    //            co_await winrt::resume_foreground(PlayingSlider().Dispatcher());
    //            PlayingSlider().Value(session.Position().count());
    //        }
    //    );
    //}


    void Player::VolumeSlider_ValueChanged(
        winrt::Windows::Foundation::IInspectable const& sender, 
        winrt::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs const& e)
    {
        ViewModel().Volume(e.NewValue());
        if (e.NewValue() <= 0)
        {
            MuteButtonSymbol().Symbol(winrt::Windows::UI::Xaml::Controls::Symbol::Mute);
            return;
        }
        MuteButtonSymbol().Symbol(winrt::Windows::UI::Xaml::Controls::Symbol::Volume);
    }

    OsuPlayer::ViewModel::PlayerViewModel Player::ViewModel()
    {
        return ViewModelLocator::Current().PlayerViewModel();
    }
}
