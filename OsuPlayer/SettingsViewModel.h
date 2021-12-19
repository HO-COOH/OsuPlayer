#pragma once

#include "SettingsViewModel.g.h"
#include <winrt/Windows.Foundation.h>
#include "SettingsModel.h"

namespace winrt::OsuPlayer::implementation
{
    struct SettingsViewModel : SettingsViewModelT<SettingsViewModel>
    {
        SettingsViewModel() = default;

        //Properties
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::hstring> Skins();

        winrt::Windows::Foundation::Collections::IObservableVector<winrt::OsuPlayer::OsuPathItem> OsuPaths();

        winrt::OsuPlayer::Mod DefaultMod() const;
        void DefaultMod(winrt::OsuPlayer::Mod mod);

        //Methods
        winrt::Windows::Foundation::IAsyncAction AddOsuPath();
        void ClearAll();

        //These following 2 methods are for supporting property change events
        winrt::event_token PropertyChanged(winrt::Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(winrt::event_token const& token) noexcept;
    private:
        inline static winrt::Windows::Foundation::Collections::IObservableVector<winrt::OsuPlayer::OsuPathItem> m_osuPathItems;
        inline static winrt::Windows::Foundation::Collections::IObservableVector<winrt::hstring> m_skinItems;
        inline static SettingsModel m_model;

        //This is for supporting property change events
        winrt::event<winrt::Windows::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct SettingsViewModel : SettingsViewModelT<SettingsViewModel, implementation::SettingsViewModel>
    {
    };
}
