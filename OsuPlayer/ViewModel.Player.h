﻿#pragma once

#include "ViewModel.PlayerViewModel.g.h"
#include <winrt/Windows.UI.Xaml.Media.h>
#include <winrt/Windows.Media.Playback.h>
#include "Utils.PropertyChangeHelper.hpp"

namespace winrt::OsuPlayer::ViewModel::implementation
{
    struct PlayerViewModel : PlayerViewModelT<PlayerViewModel>, Utils::PropertyChangeHelper<PlayerViewModel>
    {
        PlayerViewModel();
        
        winrt::Windows::Foundation::IAsyncAction Play(ViewModel::SongItemViewModel item);
        void Play();
        void PlayPrevious();
        void PlayNext();

        PlayMod Mod();
        void Mod(PlayMod mod);

        bool UseSkinHitsound();
        void UseSkinHitsound(bool useSkinHitsound);

        winrt::hstring ProgressString();
        winrt::hstring SongLengthString();

        int Progress();
        int SongLength();

        int Volume();
        void Volume(int volume);

        winrt::Windows::UI::Xaml::Media::ImageSource ImageSource();


    private:
        winrt::Windows::Media::Playback::MediaPlayer m_songPlayer;
        winrt::Windows::Media::Playback::MediaPlayer m_hitSoundPlayer;

        ViewModel::SongItemViewModel m_currentItemToPlay{ nullptr };
        int m_progress{};
        int m_length{};
    };
}

namespace winrt::OsuPlayer::ViewModel::factory_implementation
{
    struct PlayerViewModel : PlayerViewModelT<PlayerViewModel, implementation::PlayerViewModel>
    {
    };
}
