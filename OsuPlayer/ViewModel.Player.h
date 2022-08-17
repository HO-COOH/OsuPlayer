#pragma once

#include "ViewModel.PlayerViewModel.g.h"
#include <winrt/Windows.UI.Xaml.Media.h>
#include <winrt/Windows.Media.Playback.h>
#include "Utils.PropertyChangeHelper.hpp"

class HitObject;
namespace winrt::OsuPlayer::ViewModel::implementation
{
    struct PlayerViewModel : PlayerViewModelT<PlayerViewModel>, Utils::PropertyChangeHelper<PlayerViewModel>
    {
        PlayerViewModel();
        
        winrt::Windows::Foundation::IAsyncAction Play(ViewModel::SongItemViewModel item);
        void PlayCurrent();
        void PlayPrevious();
        void PlayNext();

        PlayMod Mod();
        void Mod(PlayMod mod);
        
        bool IsModEnabled();
        void IsModEnabled(bool enable);

        winrt::hstring ModString();

        bool UseSkinHitsound();
        void UseSkinHitsound(bool useSkinHitsound);

        int Progress();
        int SongLength();

        //Volumes
        int Volume();
        void Volume(int volume);
        int SongVolume();
        void SongVolume(int songVolume);
        int HitsoundVolume();
        void HitsoundVolume(int hitsoundVolume);
        void Mute();

        void Save();

        bool HasItems() { return m_currentItemToPlay != nullptr; }

        winrt::Windows::UI::Xaml::Media::ImageSource ImageSource();
        ~PlayerViewModel();
    private:
        //Update the System Media Transport Controls by directly setting properties to the PlayBackItem
        winrt::Windows::Foundation::IAsyncAction updateForSMTC(winrt::Windows::Media::Playback::MediaPlaybackItem item);

        //Update the System Media Transport Controls by directly setting SystemMediaTransportControlsDisplayUpdater
        void updateForSMTC();

        //Play the hitsound of the hitObject
        void playHitsound(HitObject const& hitObject);

        winrt::Windows::Media::Playback::MediaPlayer m_songPlayer;
        winrt::Windows::Media::Playback::MediaPlayer m_hitSoundPlayer;
        ViewModel::SongItemViewModel m_currentItemToPlay{ nullptr };
        int m_progress{};
        int m_length{};
        PlayMod m_mod{};
        
        struct MuteVolumeInfo
        {
            int songVolumeBefore{};
            int hitsoundVolumeBefore{};
            int globalVolumeBefore{};
            bool isMute = false;
        } m_muteInfo;
    };
}

namespace winrt::OsuPlayer::ViewModel::factory_implementation
{
    struct PlayerViewModel : PlayerViewModelT<PlayerViewModel, implementation::PlayerViewModel>
    {
    };
}
