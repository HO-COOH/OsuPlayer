#pragma once

#include "ViewModel.PlayerViewModel.g.h"
#include "ViewModel.SearchPageViewModel.g.h"
#include "ViewModel.SettingsViewModel.g.h"
#include "ViewModel.ColumnSettings.g.h"
#include "ViewModel.HitsoundPanelViewModel.g.h"
#include <vector>
#include <winrt/Windows.Foundation.Collections.h>
#include "ViewModel.CollectionItem.g.h"
#include <unordered_map>

class ViewModelLocator
{
	winrt::OsuPlayer::ViewModel::PlayerViewModel m_playerViewModel{ nullptr };
	winrt::OsuPlayer::ViewModel::MyMusicViewModel m_myMusicViewModel{ nullptr };
	winrt::OsuPlayer::ViewModel::SettingsViewModel m_settingsViewModel{ nullptr };
	winrt::OsuPlayer::ViewModel::ColumnSettings m_columnSettingsViewModel{ nullptr };
	//winrt::OsuPlayer::ViewModel::HitsoundPanelViewModel m_hitsoundPanelViewModel;
	winrt::Windows::Foundation::Collections::IObservableVector<winrt::OsuPlayer::ViewModel::CollectionItem> m_collections = winrt::single_threaded_observable_vector< winrt::OsuPlayer::ViewModel::CollectionItem>();
	std::unordered_map<winrt::hstring, winrt::OsuPlayer::ViewModel::MyMusicViewModel> CollectionsPageMap;
	std::unordered_map<winrt::hstring, winrt::OsuPlayer::ViewModel::CollectionItem> CollectionsNameMap;
public:
	static ViewModelLocator& Current();
	winrt::OsuPlayer::ViewModel::PlayerViewModel PlayerViewModel();
	winrt::OsuPlayer::ViewModel::MyMusicViewModel MyMusicViewModel();
	winrt::OsuPlayer::ViewModel::SettingsViewModel SettingsViewModel();
	winrt::OsuPlayer::ViewModel::ColumnSettings ColumnSettings();
	winrt::OsuPlayer::ViewModel::HitsoundPanelViewModel HitsoundPanelViewModel();
	winrt::Windows::Foundation::Collections::IObservableVector<winrt::OsuPlayer::ViewModel::CollectionItem> Collections() { return m_collections; }
	winrt::OsuPlayer::ViewModel::MyMusicViewModel getCollectionPageByCollectionItem(winrt::OsuPlayer::ViewModel::CollectionItem collectionItem);
	winrt::OsuPlayer::ViewModel::MyMusicViewModel getCollectionPageByName(winrt::hstring collectionName);
	void updateCollectionItem(winrt::OsuPlayer::ViewModel::CollectionItem& item);
};

