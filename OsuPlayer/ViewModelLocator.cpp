#include "pch.h"
#include "ViewModelLocator.h"

ViewModelLocator& ViewModelLocator::Current()
{
    static ViewModelLocator locator;
    return locator;
}

winrt::OsuPlayer::PlayerViewModel ViewModelLocator::PlayerViewModel() const
{
    return m_playerViewModel;
}

winrt::OsuPlayer::MainViewModel ViewModelLocator::MainViewModel() const
{
    return m_mainViewModel;
}

winrt::OsuPlayer::MyMusicViewModel ViewModelLocator::MyMusicViewModel() const
{
    return m_myMusicViewModel;
}

winrt::OsuPlayer::SettingsViewModel ViewModelLocator::SettingsViewModel() const
{
    return m_settingsViewModel;
}
