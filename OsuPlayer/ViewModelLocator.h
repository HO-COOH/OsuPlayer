#pragma once

#include "ViewModel.PlayerViewModel.g.h"
#include "ViewModel.MainViewModel.g.h"
#include "ViewModel.SearchPageViewModel.g.h"
#include "ViewModel.SettingsViewModel.g.h"


class ViewModelLocator
{
	winrt::OsuPlayer::ViewModel::PlayerViewModel m_playerViewModel;
	winrt::OsuPlayer::ViewModel::MainViewModel m_mainViewModel;
	winrt::OsuPlayer::ViewModel::MyMusicViewModel m_myMusicViewModel;
	winrt::OsuPlayer::ViewModel::SettingsViewModel m_settingsViewModel;

public:
	static ViewModelLocator& Current();

	winrt::OsuPlayer::ViewModel::PlayerViewModel PlayerViewModel() const;
	winrt::OsuPlayer::ViewModel::MainViewModel MainViewModel() const;
	winrt::OsuPlayer::ViewModel::MyMusicViewModel MyMusicViewModel() const;
	winrt::OsuPlayer::ViewModel::SettingsViewModel SettingsViewModel() const;
};

