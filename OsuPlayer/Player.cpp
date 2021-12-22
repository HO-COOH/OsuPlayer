#include "pch.h"
#include "Player.h"
#if __has_include("Player.g.cpp")
#include "Player.g.cpp"
#endif
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Media.Core.h>
#include <winrt/Windows.Media.Playback.h>

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::OsuPlayer::implementation
{
    Player::Player()
    {
        InitializeComponent();
        //songPlayer.AudioCategory(winrt::Windows::Media::Playback::MediaPlayerAudioCategory::Media);
        //hitsoundPlayer.AudioCategory(winrt::Windows::Media::Playback::MediaPlayerAudioCategory::SoundEffects);
    }

    void PlayerImpl::Play()
    {
        player.Source(winrt::Windows::Media::Playback::MediaPlaybackItem{ winrt::Windows::Media::Core::MediaSource::CreateFromStorageFile(currentFile) });
        player.Play();
    }

    void PlayerImpl::Play(winrt::Windows::Storage::StorageFile file)
    {
        if (currentFile && file.Path() == currentFile.Path())
            player.Play();
        else
        {
            currentFile = file;
            Play();
        }
    }

    winrt::Windows::Foundation::IAsyncAction PlayerImpl::Play(winrt::hstring path)
    {
        Play(co_await winrt::Windows::Storage::StorageFile::GetFileFromPathAsync(path));
    }

    void PlayerImpl::Pause()
    {
        player.Pause();
    }


}


winrt::Windows::Foundation::IAsyncAction winrt::OsuPlayer::implementation::Player::PlayButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
    if (isPlaying)
    {
        /*Should pause, Render a play icon*/
        PlayButtonIcon().Symbol(winrt::Windows::UI::Xaml::Controls::Symbol::Play);
        songPlayer.Pause();
    }
    else
    {
        /*Should play, render a pause icon*/
        PlayButtonIcon().Symbol(winrt::Windows::UI::Xaml::Controls::Symbol::Pause);
        co_await songPlayer.Play(LR"(D:\osu\Songs\609057 love solfege - Vanity Clock\love solfege - Vanity Clock.mp3)");
    }
    isPlaying = !isPlaying;
}


void winrt::OsuPlayer::implementation::Player::NextSongButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{

}


void winrt::OsuPlayer::implementation::Player::PreviousSongButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{

}


void winrt::OsuPlayer::implementation::Player::HalftimeCheckbox_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
    handlePlayMod(PlayMods::HalfTime);
}


void winrt::OsuPlayer::implementation::Player::DoubleTimeCheckbox_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
    handlePlayMod(PlayMods::DoubleTime);
}


void winrt::OsuPlayer::implementation::Player::NightCoreCheckbox_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
    handlePlayMod(PlayMods::NightCore);
}


void winrt::OsuPlayer::implementation::Player::UseSkinHitsoundCheckbox_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
    useSkinHitsound = true;
}



void winrt::OsuPlayer::implementation::Player::MuteButton_Click(winrt::Microsoft::UI::Xaml::Controls::SplitButton const& sender, winrt::Microsoft::UI::Xaml::Controls::SplitButtonClickEventArgs const& args)
{

}

void winrt::OsuPlayer::implementation::Player::handlePlayMod(PlayMods newPlayMod)
{
    if (newPlayMod == playMod)
        return;

    switch (newPlayMod)
    {
        case PlayMods::HalfTime:
            HalfTimeCheckbox().IsChecked(true);
            DoubleTimeCheckbox().IsChecked(false);
            NightCoreCheckbox().IsChecked(false);
            break;
        case PlayMods::DoubleTime:
            HalfTimeCheckbox().IsChecked(false);
            DoubleTimeCheckbox().IsChecked(true);
            NightCoreCheckbox().IsChecked(false);
            break;
        case PlayMods::NightCore:
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
    playMod = newPlayMod;
}


void winrt::OsuPlayer::implementation::Player::HalfTimeCheckbox_Unchecked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
    handlePlayMod(PlayMods::Normal);
}


void winrt::OsuPlayer::implementation::Player::DoubleTimeCheckbox_Unchecked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
    handlePlayMod(PlayMods::Normal);
}


void winrt::OsuPlayer::implementation::Player::NightCoreCheckbox_Unchecked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
    handlePlayMod(PlayMods::Normal);
}


void winrt::OsuPlayer::implementation::Player::UseSkinHitsoundCheckbox_Unchecked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
    useSkinHitsound = false;
}

#include <Utils.h>
#include <winrt/Windows.UI.Core.h>
void winrt::OsuPlayer::implementation::Player::Play(winrt::Windows::Storage::StorageFile song)
{
    songPlayer.Play(song);
    auto const length = songPlayer.getLength();
    SongLengthText().Text(Utils::GetDurationString(Utils::GetDuration(length)));

    PlayingSlider().Maximum(length);
    songPlayer.player.PlaybackSession().PositionChanged(
        [this](auto session, auto const& _)->winrt::Windows::Foundation::IAsyncAction
        {
            //auto const percentage = (static_cast<long double>(session.Position().count()) / session.NaturalDuration().count()) * 100.0;
            co_await winrt::resume_foreground(PlayingSlider().Dispatcher());
            PlayingSlider().Value(session.Position().count());
        }
    );
}

void winrt::OsuPlayer::implementation::Player::Play(SongItem item)
{
    //Play(item.ViewModel().SongFile());
    //songPlayer.player.Source(item.ViewModel().SongFile());
    songPlayer.player.Play();
    //CoverImage().Source(item.Image());
}


void winrt::OsuPlayer::implementation::Player::VolumeSlider_ValueChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs const& e)
{
    if (e.NewValue() <= 0)
    {
        MuteButtonSymbol().Symbol(winrt::Windows::UI::Xaml::Controls::Symbol::Mute);
        return;
    }
    

    MuteButtonSymbol().Symbol(winrt::Windows::UI::Xaml::Controls::Symbol::Volume);
}
