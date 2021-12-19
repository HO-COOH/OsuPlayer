#pragma once

#include "winrt/Windows.UI.Xaml.h"
#include "winrt/Windows.UI.Xaml.Markup.h"
#include "winrt/Windows.UI.Xaml.Interop.h"
#include "winrt/Windows.UI.Xaml.Media.h"
#include "winrt/Windows.UI.Xaml.Controls.Primitives.h"
#include "winrt/Windows.Media.Playback.h"
#include "Player.g.h"
#include "PlayMods.h"
#include "SongItem.g.h"

/*
    According to microsoft: https://docs.microsoft.com/zh-cn/uwp/api/windows.ui.xaml.media.imagesource?view=winrt-22000

*/
namespace winrt::OsuPlayer::implementation
{
    class PlayerImpl
    {
    public:
        winrt::Windows::Media::Playback::MediaPlayer player{};
        winrt::Windows::Storage::StorageFile currentFile{nullptr};
    public:
        void Play();
        void Play(winrt::Windows::Storage::StorageFile file);
        winrt::Windows::Foundation::IAsyncAction Play(winrt::hstring path);
        void Pause();
        auto getLength()
        {
            return player.PlaybackSession().NaturalDuration().count();
        }
    };

    struct Player : PlayerT<Player>
    {
        Player();
        winrt::Windows::Foundation::IAsyncAction PlayButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void NextSongButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void PreviousSongButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void HalftimeCheckbox_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void DoubleTimeCheckbox_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void NightCoreCheckbox_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void UseSkinHitsoundCheckbox_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void MuteButton_Click(winrt::Microsoft::UI::Xaml::Controls::SplitButton const& sender, winrt::Microsoft::UI::Xaml::Controls::SplitButtonClickEventArgs const& args); 
        winrt::Windows::UI::Xaml::Media::ImageSource ImageSource() { return m_imageSource; }
    private:
        PlayMods playMod = PlayMods::Normal;
        bool isPlaying = false;
        bool useSkinHitsound{};
        void handlePlayMod(PlayMods newPlayMod);
        PlayerImpl songPlayer{};
        PlayerImpl hitsoundPlayer{};
        winrt::Windows::UI::Xaml::Media::ImageSource m_imageSource{nullptr};
    public:
        void HalfTimeCheckbox_Unchecked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void DoubleTimeCheckbox_Unchecked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void NightCoreCheckbox_Unchecked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void UseSkinHitsoundCheckbox_Unchecked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void Play(winrt::Windows::Storage::StorageFile song);
        void Play(SongItem item);
        void VolumeSlider_ValueChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs const& e);
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct Player : PlayerT<Player, implementation::Player>
    {
    };
}
