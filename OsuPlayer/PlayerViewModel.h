#pragma once

#include "SongItem.g.h"
#include "PlayerViewModel.g.h"
#include <winrt/Windows.UI.Xaml.Media.h>

namespace winrt::OsuPlayer::implementation
{
    struct PlayerViewModel : PlayerViewModelT<PlayerViewModel>
    {
        PlayerViewModel() = default;
        
        void Play(SongItem item);
        void Play();
        void PlayPrevious();
        void PlayNext();

        PlayMod Mod();
        void Mod(PlayMod mod);

        bool UseSkinHitsound();
        void UseSkinHitsound(bool useSkinHitsound);

        int Volume();
        void Volume(int volume);

        winrt::Windows::UI::Xaml::Media::ImageSource ImageSource();
    private:

    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct PlayerViewModel : PlayerViewModelT<PlayerViewModel, implementation::PlayerViewModel>
    {
    };
}
