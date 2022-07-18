#pragma once

#include "Settings.g.h"
#include "ViewModel.SettingsViewModel.g.h"
#include "Utils.PropertyChangeHelper.hpp"

namespace winrt::OsuPlayer::implementation
{
    struct Settings : SettingsT<Settings>, Utils::PropertyChangeHelper<Settings>
    {
        Settings();

        OsuPlayer::ViewModel::SettingsViewModel ViewModel();


        void DefaultHitsoundButton_Click(
            winrt::Microsoft::UI::Xaml::Controls::SplitButton const& sender, 
            winrt::Microsoft::UI::Xaml::Controls::SplitButtonClickEventArgs const& args
        );

        winrt::hstring DefaultModString();
        void HalfTimeItem_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void DoubleTimeItem_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void NightCoreItem_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        winrt::Windows::Foundation::IAsyncAction NewFolderButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct Settings : SettingsT<Settings, implementation::Settings>
    {
    };
}
