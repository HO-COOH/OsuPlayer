#include "pch.h"
#include "Player.h"
#if __has_include("Player.g.cpp")
#include "Player.g.cpp"
#endif


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
        m_model.Play();
    }


    void Player::NextSongButton_Click(
        winrt::Windows::Foundation::IInspectable const& sender, 
        winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        m_model.PlayNext();
    }


    void Player::PreviousSongButton_Click(
        winrt::Windows::Foundation::IInspectable const& sender, 
        winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        m_model.PlayPrevious();
    }


    void Player::HalftimeCheckbox_Checked(
        winrt::Windows::Foundation::IInspectable const& sender, 
        winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        handlePlayMod(PlayMod::HalfTime);
    }


    void Player::DoubleTimeCheckbox_Checked(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        handlePlayMod(PlayMod::DoubleTime);
    }


    void Player::NightCoreCheckbox_Checked(
        winrt::Windows::Foundation::IInspectable const& sender, 
        winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        handlePlayMod(PlayMod::NightCore);
    }


    void Player::UseSkinHitsoundCheckbox_Checked(
        winrt::Windows::Foundation::IInspectable const& sender, 
        winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        m_model.UseSkinHitsound(true);
    }

    void Player::MuteButton_Click(
        winrt::Microsoft::UI::Xaml::Controls::SplitButton const& sender, 
        winrt::Microsoft::UI::Xaml::Controls::SplitButtonClickEventArgs const& args)
    {
        m_model.Volume(0);
    }

    void Player::handlePlayMod(PlayMod newPlayMod)
    {
        if (newPlayMod == m_model.Mod())
            return;

        switch (newPlayMod)
        {
            case PlayMod::HalfTime:
                HalfTimeCheckbox().IsChecked(true);
                DoubleTimeCheckbox().IsChecked(false);
                NightCoreCheckbox().IsChecked(false);
                break;
            case PlayMod::DoubleTime:
                HalfTimeCheckbox().IsChecked(false);
                DoubleTimeCheckbox().IsChecked(true);
                NightCoreCheckbox().IsChecked(false);
                break;
            case PlayMod::NightCore:
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
        m_model.Mod(newPlayMod);
    }


    void Player::HalfTimeCheckbox_Unchecked(
        winrt::Windows::Foundation::IInspectable const& sender, 
        winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        handlePlayMod(PlayMod::Normal);
    }


    void Player::DoubleTimeCheckbox_Unchecked(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        handlePlayMod(PlayMod::Normal);
    }


    void Player::NightCoreCheckbox_Unchecked(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        handlePlayMod(PlayMod::Normal);
    }

    void Player::UseSkinHitsoundCheckbox_Unchecked(
        winrt::Windows::Foundation::IInspectable const& sender, 
        winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        m_model.UseSkinHitsound(false);
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
        m_model.Volume(e.NewValue());
        if (e.NewValue() <= 0)
        {
            MuteButtonSymbol().Symbol(winrt::Windows::UI::Xaml::Controls::Symbol::Mute);
            return;
        }
        MuteButtonSymbol().Symbol(winrt::Windows::UI::Xaml::Controls::Symbol::Volume);
    }

    OsuPlayer::PlayerViewModel Player::ViewModel()
    {
        return m_model;
    }
}
