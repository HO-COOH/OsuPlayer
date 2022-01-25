#pragma once

#include "SongItem.g.h"
#include "PlayerViewModel.g.h"
#include <winrt/Windows.UI.Xaml.Media.h>
#include "PlayerModel.h"

namespace winrt::OsuPlayer::implementation
{
    struct PlayerViewModel : PlayerViewModelT<PlayerViewModel>
    {
        PlayerViewModel();
        
        void Play(SongItem item);
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

        //These following 2 methods are for supporting property change events
        winrt::event_token PropertyChanged(winrt::Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(winrt::event_token const& token) noexcept;

    private:
        SongItem m_currentItemToPlay;
        PlayerModel m_model;
        int m_progress{};
        int m_length{};
        //This is for supporting property change events
        winrt::event<winrt::Windows::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct PlayerViewModel : PlayerViewModelT<PlayerViewModel, implementation::PlayerViewModel>
    {
    };
}
