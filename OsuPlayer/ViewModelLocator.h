#pragma once

#include "PlayerViewModel.g.h"
#include "MainViewModel.g.h"
#include "SearchPageViewModel.g.h"
#include "SettingsViewModel.g.h"


class ViewModelLocator
{
	winrt::OsuPlayer::PlayerViewModel m_playerViewModel;
	winrt::OsuPlayer::MainViewModel m_mainViewModel;
	winrt::OsuPlayer::MyMusicViewModel m_myMusicViewModel;
	winrt::OsuPlayer::SettingsViewModel m_settingsViewModel;

public:
	static ViewModelLocator& Current();

	winrt::OsuPlayer::PlayerViewModel PlayerViewModel() const;
	winrt::OsuPlayer::MainViewModel MainViewModel() const;
	winrt::OsuPlayer::MyMusicViewModel MyMusicViewModel() const;
	winrt::OsuPlayer::SettingsViewModel SettingsViewModel() const;
};

