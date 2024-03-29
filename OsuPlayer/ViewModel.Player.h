﻿#pragma once

#include "ViewModel.PlayerViewModel.g.h"
#include <winrt/Windows.UI.Xaml.Media.h>
#include <winrt/Windows.Media.Playback.h>
#include "Utils.PropertyChangeHelper.hpp"
#include "HitsoundPlayer.h"

struct HitObject;
namespace winrt::OsuPlayer::ViewModel::implementation
{
    struct PlayerViewModel : PlayerViewModelT<PlayerViewModel>, Utils::PropertyChangeHelper<PlayerViewModel>
    {
        PlayerViewModel();
        
        winrt::Windows::Foundation::IAsyncAction Play(ViewModel::SongItemViewModel item);
        void PlayCurrent();
        void PlayPrevious();
        void PlayNext();

        winrt::Windows::Foundation::IAsyncAction PlayList(winrt::Windows::Media::Playback::MediaPlaybackList list);

        PlayMod Mod();
        void Mod(PlayMod mod);
        
        bool IsModEnabled();
        void IsModEnabled(bool enable);


        bool UseSkinHitsound();
        void UseSkinHitsound(bool useSkinHitsound);

        int Progress();
        int SongLength();

        //Volumes
        int Volume();
        void Volume(int volume);
        int SongVolume();
        void SongVolume(int songVolume, bool writeToSetting = true);
        int HitsoundVolume();
        void HitsoundVolume(int hitsoundVolume, bool writeToSetting = true);
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

        winrt::Windows::Media::Playback::MediaPlayer m_songPlayer;
        HitsoundPlayer m_hitSoundPlayer;
        ViewModel::SongItemViewModel m_currentItemToPlay{ nullptr };
        int m_progress{};
        int m_length{};
        int m_mixedVolume{};
        
        struct MuteVolumeInfo
        {
            double songVolumeBefore{};
            double hitsoundVolumeBefore{};
            double globalVolumeBefore{};
            bool isMute = false;
        } m_muteInfo;

        std::vector<TimingPoint>::const_iterator m_timingPointIter{};
    };
}

namespace winrt::OsuPlayer::ViewModel::factory_implementation
{
    struct PlayerViewModel : PlayerViewModelT<PlayerViewModel, implementation::PlayerViewModel>
    {
    };
}
