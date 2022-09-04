#pragma once

#include "ViewModel.SettingsViewModel.g.h"
#include <winrt/Windows.Foundation.h>
#include "Utils.PropertyChangeHelper.hpp"
#include "Model.Folders.h"
#include "Model.Skin.h"

namespace winrt::OsuPlayer::ViewModel::implementation
{
    struct SettingsViewModel : SettingsViewModelT<SettingsViewModel>, Utils::PropertyChangeHelper<SettingsViewModel>
    {
        SettingsViewModel();

        //Properties
        winrt::Windows::Foundation::Collections::IObservableVector<ViewModel::OsuPathItemViewModel> OsuPaths();


        int ThemeIndex();
        void ThemeIndex(int theme);

        bool IsModEnabled();
        void IsModEnabled(bool enable);

        Mod DefaultMod() const;
        void DefaultMod(Mod mod);

        bool HitsoundGlobalEnabled() { return m_hitsoundGlobalEnabled; }
        void HitsoundGlobalEnabled(bool enabled);

        winrt::Windows::Storage::StorageFolder SelectedSkin();
        void SelectedSkin(winrt::Windows::Storage::StorageFolder selectedSkin);

        int OsuPathActionIndex();
        void OsuPathActionIndex(int osuPathAction);

        int Offset();
        void Offset(int offset);

        int LinkActionIndex();
        void LinkActionIndex(int linkAction);

        winrt::hstring CustomSearchPrefix();
        void CustomSearchPrefix(winrt::hstring prefix);

        winrt::Windows::UI::Xaml::Visibility CustomSearchTextBoxVisibility();

        int JumpListRecentSongs();
        void JumpListRecentSongs(int numSongs);

        int JumpListRecentCollections();
        void JumpListRecentCollections(int numCollections);

        bool AllowModifyOsuData();
        void AllowModifyOsuData(bool allow);
        
        //Methods
        winrt::Windows::Foundation::IAsyncOperation<AddOsuFolderResult> AddOsuPath();

        winrt::hstring ExperimentSettingHeaderText();

        
    private:
        int m_theme;
        int m_osuPathAction;
        int m_linkAction;
        winrt::hstring m_customSearchPrefix;
        winrt::Windows::Storage::StorageFolder m_selectedSkin{ nullptr };
        Mod m_mod;
        int m_jumplistRecentSongs;
        int m_jumplistRecentCollections;
        bool m_allowModifyOsuData;
        bool m_hitsoundGlobalEnabled;
        int m_offset;
        static winrt::Windows::Storage::ApplicationDataContainer m_localSettings;
        winrt::Windows::Foundation::Collections::IObservableVector<ViewModel::OsuPathItemViewModel> m_osuPathItems = winrt::single_threaded_observable_vector<ViewModel::OsuPathItemViewModel>();
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
