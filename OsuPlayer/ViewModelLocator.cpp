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
