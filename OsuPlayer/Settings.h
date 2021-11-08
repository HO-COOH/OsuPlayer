#pragma once

#include "Settings.g.h"
#include <optional>

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
        static winrt::Windows::Foundation::Collections::IObservableVector<winrt::hstring> m_osuPath;
        //ApplicationTheme theme = Application::Current().RequestedTheme();
        static winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Storage::StorageFolder> DoPickOsuFolder();
    public:
        winrt::Windows::Foundation::IAsyncAction ListBox_SelectionChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Controls::SelectionChangedEventArgs const& e);
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct Settings : SettingsT<Settings, implementation::Settings>
    {
    };
}
