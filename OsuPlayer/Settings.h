#pragma once

#include "Settings.g.h"
#include "SongItem.g.h"
#include "MyMusicModel.h"
#include "OsuPathItem.g.h"
namespace winrt::OsuPlayer::implementation
{
    struct Settings : SettingsT<Settings>
    {
        Settings();

        winrt::Windows::Foundation::Collections::IObservableVector<winrt::OsuPlayer::OsuPathItem> osuPath();

        void LightButton_Checked(
            winrt::Windows::Foundation::IInspectable const& sender,
            winrt::Windows::UI::Xaml::RoutedEventArgs const& e
        );
        
        void DarkButton_Checked(
            winrt::Windows::Foundation::IInspectable const& sender,
            winrt::Windows::UI::Xaml::RoutedEventArgs const& e
        );

        void SystemThemeButton_Checked(
            winrt::Windows::Foundation::IInspectable const& sender, 
            winrt::Windows::UI::Xaml::RoutedEventArgs const& e
        );

        /*winrt::Windows::Foundation::IAsyncAction*/ void ListBox_SelectionChanged(
            winrt::Windows::Foundation::IInspectable const& sender,
            winrt::Windows::UI::Xaml::Controls::SelectionChangedEventArgs const& e
        );

        static winrt::event_token s_osuPathChangedHandler(winrt::OsuPlayer::OsuPathChangedHandler const& handler);
        static void s_osuPathChangedHandler(winrt::event_token const& token) noexcept;
        static winrt::Windows::Foundation::Collections::IObservableVector<winrt::OsuPlayer::SongItem> s_songItems() { return m_songItems; };
    private:
        static winrt::Windows::Foundation::Collections::IObservableVector<winrt::OsuPlayer::OsuPathItem> m_osuPath;
       
        static winrt::Windows::Foundation::Collections::IObservableVector<winrt::OsuPlayer::SongItem> m_songItems;
        static winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Storage::StorageFolder> DoPickOsuFolder();
        static winrt::event<OsuPathChangedHandler> s_handlers;
        static MyMusicModel model;
    public:
        static winrt::Windows::Foundation::IAsyncAction AddOsuPathButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void ClearAllButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void StoreAllCheckbox_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void StoreAllCheckbox_Unchecked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void StoreAllCheckbox_Indeterminate(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void DefaultHitsoundButton_Click(winrt::Microsoft::UI::Xaml::Controls::SplitButton const& sender, winrt::Microsoft::UI::Xaml::Controls::SplitButtonClickEventArgs const& args);

        void HalftimeCheckbox_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void DoubleTimeCheckbox_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void NightCoreCheckbox_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct Settings : SettingsT<Settings, implementation::Settings>
    {
    };
}
