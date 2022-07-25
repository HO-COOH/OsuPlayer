#pragma once

#include "ViewModel.PlayerViewModel.g.h"
#include "ViewModel.SearchPageViewModel.g.h"
#include "ViewModel.SettingsViewModel.g.h"
#include "ViewModel.ColumnSettings.g.h"

class ViewModelLocator
{
	winrt::OsuPlayer::ViewModel::PlayerViewModel m_playerViewModel;
	winrt::OsuPlayer::ViewModel::MyMusicViewModel m_myMusicViewModel;
	winrt::OsuPlayer::ViewModel::SettingsViewModel m_settingsViewModel;
	winrt::OsuPlayer::ViewModel::ColumnSettings m_columnSettingsViewModel;
public:
	static ViewModelLocator& Current();

	winrt::OsuPlayer::ViewModel::PlayerViewModel PlayerViewModel() const;
	winrt::OsuPlayer::ViewModel::MyMusicViewModel MyMusicViewModel() const;
	winrt::OsuPlayer::ViewModel::SettingsViewModel SettingsViewModel() const;
	winrt::OsuPlayer::ViewModel::ColumnSettings ColumnSettings() const;
};

