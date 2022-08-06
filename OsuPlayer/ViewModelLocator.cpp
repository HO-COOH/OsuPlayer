#include "pch.h"
#include "ViewModelLocator.h"

ViewModelLocator& ViewModelLocator::Current()
{
    static ViewModelLocator locator;
    return locator;
}

winrt::OsuPlayer::ViewModel::PlayerViewModel ViewModelLocator::PlayerViewModel() const
{
    return m_playerViewModel;
}

winrt::OsuPlayer::ViewModel::MyMusicViewModel ViewModelLocator::MyMusicViewModel() const
{
    return m_myMusicViewModel;
}

winrt::OsuPlayer::ViewModel::SettingsViewModel ViewModelLocator::SettingsViewModel() const
{
    return m_settingsViewModel;
}

winrt::OsuPlayer::ViewModel::ColumnSettings ViewModelLocator::ColumnSettings() const
{
    return m_columnSettingsViewModel;
}

winrt::OsuPlayer::ViewModel::HitsoundPanelViewModel ViewModelLocator::HitsoundPanelViewModel() const
{
    static winrt::OsuPlayer::ViewModel::HitsoundPanelViewModel model;
    return model;
}
