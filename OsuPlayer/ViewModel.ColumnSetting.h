#pragma once
#include "ViewModel.ColumnSettingItem.g.h"
#include "ViewModel.ColumnSettings.g.h"
#include "Utils.PropertyChangeHelper.hpp"
#include <winrt/Windows.Storage.h>

namespace winrt::OsuPlayer::ViewModel::implementation
{
    struct ColumnSettingItem : ColumnSettingItemT<ColumnSettingItem>, Utils::PropertyChangeHelper<ColumnSettingItem>
    {
        ColumnSettingItem() = default;
        ColumnSettingItem(bool show, winrt::hstring name) : m_show{ show }, m_name{ name } {}

        bool Show() { return m_show; }
        void Show(bool show);

        winrt::hstring Name() { return m_name; }
        void Name(winrt::hstring name) { m_name = name; }
    private:
        bool m_show = true;
        winrt::hstring m_name;
    };

    struct ColumnSettings : ColumnSettingsT<ColumnSettings>, Utils::PropertyChangeHelper<ColumnSettings>
    {
        ColumnSettings();

        winrt::Windows::Foundation::Collections::IObservableVector<ViewModel::ColumnSettingItem> Items() { return m_items; }

        //Properties for the buttons
        int SelectedIndex();
        void SelectedIndex(int selectedIndex);
        bool IsMoveUpEnabled();
        bool IsMoveDownEnabled();
        bool IsShowEnabled();
        bool IsHideEnabled();

        //Methods for the buttons
        void MoveUpItem();
        void MoveDownItem();
        void ShowItem();
        void HideItem();
        void UpdateVisibilityButtonStatus();

        //Save
        void Save();
    private:
        int m_selectedIndex = -1;
        void assertIndexValid() const { assert(m_selectedIndex != -1 && m_selectedIndex < m_items.Size()); }
        winrt::Windows::Foundation::Collections::IObservableVector<ViewModel::ColumnSettingItem> m_items = winrt::single_threaded_observable_vector<ViewModel::ColumnSettingItem>();
        static winrt::Windows::Storage::ApplicationDataContainer m_localSettings;
    };
}

namespace winrt::OsuPlayer::ViewModel::factory_implementation
{
    struct ColumnSettingItem : ColumnSettingItemT<ColumnSettingItem, implementation::ColumnSettingItem>
    {
    };

    struct ColumnSettings : ColumnSettingsT<ColumnSettings, implementation::ColumnSettings>
    {
    };
}