#pragma once

#include "ViewModel.SettingsViewModel.g.h"
#include <winrt/Windows.Foundation.h>
#include "Utils.PropertyChangeHelper.hpp"
#include "Model.Folders.h"

namespace winrt::OsuPlayer::ViewModel::implementation
{
    struct SettingsViewModel : SettingsViewModelT<SettingsViewModel>, Utils::PropertyChangeHelper<SettingsViewModel>
    {
        SettingsViewModel();

        //Properties
        winrt::Windows::Foundation::Collections::IObservableVector<OsuPlayer::OsuPathItem> OsuPaths();


        int ThemeIndex();
        void ThemeIndex(int theme);

        bool IsModEnabled();
        void IsModEnabled(bool enable);

        winrt::Windows::Foundation::Collections::IObservableVector<winrt::hstring> Skins();

 

        Mod DefaultMod() const;
        void DefaultMod(Mod mod);

        int JumpListRecentSongs();
        void JumpListRecentSongs(int numSongs);

        int JumpListRecentCollections();
        void JumpListRecentCollections(int numCollections);

        bool AllowModifyOsuData();
        void AllowModifyOsuData(bool allow);
        
        //Methods
        winrt::Windows::Foundation::IAsyncOperation<AddOsuFolderResult> AddOsuPath();

        
    private:
        int m_theme;
        Mod m_mod;
        int m_jumplistRecentSongs;
        int m_jumplistRecentCollections;
        bool m_allowModifyOsuData;
        static winrt::Windows::Storage::ApplicationDataContainer m_localSettings;
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::OsuPlayer::OsuPathItem> m_osuPathItems = winrt::single_threaded_observable_vector<winrt::OsuPlayer::OsuPathItem>();
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::hstring> m_skinItems;
        void setTheme();
        void loadOsuPaths();
        static Model::Folders& GetFolderData() { return Model::Folders::GetInstance(); }
    };
}

namespace winrt::OsuPlayer::ViewModel::factory_implementation
{
    struct SettingsViewModel : SettingsViewModelT<SettingsViewModel, implementation::SettingsViewModel>
    {
    };
}
