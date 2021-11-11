#pragma once

#include "Settings.g.h"
namespace winrt::OsuPlayer::implementation
{
    struct Settings : SettingsT<Settings>
    {
        Settings();

        winrt::Windows::Foundation::Collections::IObservableVector<winrt::hstring> osuPath();

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

        winrt::Windows::Foundation::IAsyncAction ListBox_SelectionChanged(
            winrt::Windows::Foundation::IInspectable const& sender,
            winrt::Windows::UI::Xaml::Controls::SelectionChangedEventArgs const& e
        );

        static winrt::event_token s_osuPathChangedHandler(winrt::OsuPlayer::OsuPathChangedHandler const& handler);
        static void s_osuPathChangedHandler(winrt::event_token const& token) noexcept;

    private:
        static winrt::Windows::Foundation::Collections::IObservableVector<winrt::hstring> m_osuPath;
        static winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Storage::StorageFolder> DoPickOsuFolder();
        static winrt::event<OsuPathChangedHandler> s_handlers;
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct Settings : SettingsT<Settings, implementation::Settings>
    {
    };
}
