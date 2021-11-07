#pragma once

#include "Settings.g.h"

namespace winrt::OsuPlayer::implementation
{
    struct Settings : SettingsT<Settings>
    {
        Settings();

        winrt::Windows::Foundation::Collections::IObservableVector<winrt::hstring> osuPath();
        void LightButton_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void DarkButton_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void SystemThemeButton_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
    
    private:
        //ApplicationTheme theme = Application::Current().RequestedTheme();
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct Settings : SettingsT<Settings, implementation::Settings>
    {
    };
}
