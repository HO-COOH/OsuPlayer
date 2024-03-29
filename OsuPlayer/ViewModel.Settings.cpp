﻿#include "pch.h"
#include "ViewModel.Settings.h"
#if __has_include("ViewModel.SettingsViewModel.g.cpp")
#include "ViewModel.SettingsViewModel.g.cpp"
#endif

#include <winrt/Windows.UI.Xaml.Controls.h>
#include <winrt/Windows.Storage.Pickers.h>
#include "PlayMods.h"
#include "Model.MyMusic.h"
#include "Utils.h"
#include "Utils.ThemeHelper.h"
#include <ppltasks.h>
#include <pplawait.h>
#include <winrt/Windows.ApplicationModel.Resources.h>
namespace winrt::OsuPlayer::ViewModel::implementation
{
	winrt::Windows::Storage::ApplicationDataContainer SettingsViewModel::m_localSettings = winrt::Windows::Storage::ApplicationData::Current().LocalSettings();

	SettingsViewModel::SettingsViewModel() :
		m_theme(winrt::unbox_value_or<int>(m_localSettings.Values().TryLookup(L"Theme"), 0)),
		m_osuPathAction(winrt::unbox_value_or<int>(m_localSettings.Values().TryLookup(L"OsuPathAction"), 0)),
		m_linkAction(winrt::unbox_value_or<int>(m_localSettings.Values().TryLookup(L"LinkAction"), 2)),
		m_customSearchPrefix(winrt::unbox_value_or<winrt::hstring>(m_localSettings.Values().TryLookup(L"CustomSearchPrefix"), L"")),
		m_mod(static_cast<Mod>(winrt::unbox_value_or<int>(m_localSettings.Values().TryLookup(L"Mod"), 0))),
		m_jumplistRecentSongs(winrt::unbox_value_or<int>(m_localSettings.Values().TryLookup(L"RecentSongs"), 0)),
		m_jumplistRecentCollections(winrt::unbox_value_or<int>(m_localSettings.Values().TryLookup(L"RecentCollections"), 0)),
		m_allowModifyOsuData(winrt::unbox_value_or<bool>(m_localSettings.Values().TryLookup(L"AllowModifyOsuData"), false)),
		m_hitsoundGlobalEnabled(winrt::unbox_value_or<bool>(m_localSettings.Values().TryLookup(L"HitsoundEnable"), false)),
		m_offset(winrt::unbox_value_or<int>(m_localSettings.Values().TryLookup(L"Offset"), 0)),
		m_useOriginalLanguage(winrt::unbox_value_or<int>(m_localSettings.Values().TryLookup(L"UseOriginalLanguage"), 0)),
		m_useInternalBrowser(winrt::unbox_value_or<bool>(m_localSettings.Values().TryLookup(L"UseInternalBrowser"), true))
	{
		loadOsuPaths();
	}

	winrt::Windows::Foundation::IAsyncOperation<AddOsuFolderResult> SettingsViewModel::AddOsuPath()
	{
		winrt::Windows::Storage::Pickers::FolderPicker picker;
		picker.ViewMode(winrt::Windows::Storage::Pickers::PickerViewMode::List);

		auto folder = co_await picker.PickSingleFolderAsync();
		
		if (!folder)
			co_return AddOsuFolderResult::Invalid;

		if (!GetFolderData().isFolderUnique(folder))
			co_return AddOsuFolderResult::Duplicate;

		if (co_await GetFolderData().addOsuFolder(folder))
		{
			OsuPathItemViewModel model;
			model.Path(folder.Path());
			OsuPaths().Append(model);
			co_await concurrency::create_task([] { Model::MyMusicModel::GetInstance().startIndexing(); });
			co_await concurrency::create_task([this, folder]()
			{
				Model::Skins::GetInstance().add(folder.GetFolderAsync(L"Skins").get());
			});
			co_return AddOsuFolderResult::Success;
		}
		co_return AddOsuFolderResult::Invalid;
	}

	winrt::hstring SettingsViewModel::ExperimentSettingHeaderText()
	{
		return winrt::Windows::ApplicationModel::Resources::ResourceLoader::GetForCurrentView().GetString(
			m_allowModifyOsuData ? L"EnableText" : L"DisableText");
	}

	void implementation::SettingsViewModel::setTheme()
	{
		switch (m_theme)
		{
			case 0:  return Utils::ThemeHelper::RootTheme(winrt::Windows::UI::Xaml::ElementTheme::Light);
			case 1:  return Utils::ThemeHelper::RootTheme(winrt::Windows::UI::Xaml::ElementTheme::Dark);
			default: return Utils::ThemeHelper::RootTheme(winrt::Windows::UI::Xaml::ElementTheme::Default);
		}
	}

	void implementation::SettingsViewModel::loadOsuPaths()
	{
		auto pathsValue = m_localSettings.Values().TryLookup(L"Paths");
		if (pathsValue)
		{
			for (auto path : Utils::SplitPathFromString(pathsValue.as<winrt::hstring>()))
			{
				winrt::OsuPlayer::ViewModel::OsuPathItemViewModel pathItem;
				pathItem.Path(path);
				OsuPaths().Append(pathItem);
			}
		}
	}


	int implementation::SettingsViewModel::ThemeIndex()
	{
		return m_theme;
	}

	void implementation::SettingsViewModel::ThemeIndex(int theme)
	{
		if (theme != m_theme)
		{
			m_theme = theme;
			m_localSettings.Values().Insert(L"Theme", winrt::box_value(theme));
			setTheme();
		}
	}

	bool SettingsViewModel::IsModEnabled()
	{
		return m_mod != Mod::Normal;
	}

	void SettingsViewModel::IsModEnabled(bool enable)
	{
		if (!enable)
			m_mod = Mod::Normal;
		m_localSettings.Values().Insert(L"Mod", winrt::box_value(static_cast<int>(enable? m_mod : Mod::Normal)));
	}


	winrt::Windows::Foundation::Collections::IObservableVector<ViewModel::OsuPathItemViewModel> SettingsViewModel::OsuPaths()
	{
		return m_osuPathItems;
	}

	Mod SettingsViewModel::DefaultMod() const
	{
		return m_mod;
	}

	void SettingsViewModel::DefaultMod(Mod mod)
	{
		if (mod != m_mod)
		{
			m_mod = mod;
			m_localSettings.Values().Insert(L"Mod", winrt::box_value(static_cast<int>(m_mod)));
			raisePropertyChange(L"IsModEnabled");
		}
	}

	void SettingsViewModel::HitsoundGlobalEnabled(bool enabled)
	{
		if (m_hitsoundGlobalEnabled != enabled)
		{
			m_hitsoundGlobalEnabled = enabled;
			m_localSettings.Values().Insert(L"HitsoundEnable", winrt::box_value(enabled));
			raisePropertyChange(L"HitsoundGlobalEnabled");
		}
	}

	winrt::Windows::Storage::StorageFolder SettingsViewModel::SelectedSkin()
	{
		return m_selectedSkin;
	}

	void SettingsViewModel::SelectedSkin(winrt::Windows::Storage::StorageFolder selectedSkin)
	{
		m_selectedSkin = selectedSkin;
		raisePropertyChange(L"SelectedSkin");
	}

	int SettingsViewModel::OsuPathActionIndex()
	{
		return m_osuPathAction;
	}

	void SettingsViewModel::OsuPathActionIndex(int osuPathAction)
	{
		if (m_osuPathAction != osuPathAction)
		{
			m_osuPathAction = osuPathAction;
			m_localSettings.Values().Insert(L"OsuPathAction", winrt::box_value(osuPathAction));
		}
	}

	int SettingsViewModel::Offset()
	{
		return m_offset;
	}

	void SettingsViewModel::Offset(int offset)
	{
		if (offset != m_offset)
		{
			m_offset = offset;
			m_localSettings.Values().Insert(L"Offset", winrt::box_value(offset));
		}
	}

	int implementation::SettingsViewModel::LinkActionIndex()
	{
		return m_linkAction;
	}
	void implementation::SettingsViewModel::LinkActionIndex(int linkAction)
	{
		if (linkAction != m_linkAction)
		{
			m_linkAction = linkAction;
			m_localSettings.Values().Insert(L"LinkAction", winrt::box_value(m_linkAction));
			raisePropertyChange(L"CustomSearchTextBoxVisibility");
			raisePropertyChange(L"UserInternalBrowserVisibility");
		}
	}
	winrt::hstring SettingsViewModel::CustomSearchPrefix()
	{
		return m_customSearchPrefix;
	}
	void SettingsViewModel::CustomSearchPrefix(winrt::hstring prefix)
	{
		if (m_customSearchPrefix != prefix)
		{
			m_customSearchPrefix = prefix;
			m_localSettings.Values().Insert(L"CustomSearchPrefix", winrt::box_value(prefix));
		}
	}
	winrt::Windows::UI::Xaml::Visibility implementation::SettingsViewModel::CustomSearchTextBoxVisibility()
	{
		return m_linkAction == 3 ?
			winrt::Windows::UI::Xaml::Visibility::Visible :
			winrt::Windows::UI::Xaml::Visibility::Collapsed;
	}
	int implementation::SettingsViewModel::JumpListRecentSongs()
	{
		return m_jumplistRecentSongs;
	}
	void implementation::SettingsViewModel::JumpListRecentSongs(int numSongs)
	{
		if (m_jumplistRecentSongs != numSongs)
		{
			m_jumplistRecentSongs = numSongs;
			m_localSettings.Values().Insert(L"RecentSongs", winrt::box_value(numSongs));
		}
	}
	int implementation::SettingsViewModel::JumpListRecentCollections()
	{
		return m_jumplistRecentCollections;
	}
	void implementation::SettingsViewModel::JumpListRecentCollections(int numCollections)
	{
		if (m_jumplistRecentCollections != numCollections)
		{
			m_jumplistRecentCollections = numCollections;
			m_localSettings.Values().Insert(L"RecentCollections", winrt::box_value(numCollections));
		}
	}
	bool implementation::SettingsViewModel::AllowModifyOsuData()
	{
		return m_allowModifyOsuData;
	}
	void implementation::SettingsViewModel::AllowModifyOsuData(bool allow)
	{
		if (allow != m_allowModifyOsuData)
		{
			m_allowModifyOsuData = allow;
			m_localSettings.Values().Insert(L"AllowModifyOsuData", winrt::box_value(allow));
			raisePropertyChange(L"ExperimentSettingHeaderText");	//because there is a header text that binds to this property
		}
	}
	bool SettingsViewModel::UseOriginalLanguage()
	{
		return m_useOriginalLanguage;
	}

	void SettingsViewModel::UseOriginalLanguage(bool value)
	{
		if (m_useOriginalLanguage != value)
		{
			m_useOriginalLanguage = value;
			m_localSettings.Values().Insert(L"UseOriginalLanguage", winrt::box_value(value));
		}
	}

	void SettingsViewModel::UseInternalBrowser(bool value)
	{
		if (value != m_useInternalBrowser)
		{
			m_useInternalBrowser = value;
			m_localSettings.Values().Insert(L"UseInternalBrowser", winrt::box_value(value));
		}
	}

	winrt::Windows::UI::Xaml::Visibility SettingsViewModel::UseInternalBrowserVisibility()
	{
		return m_linkAction == 0 || m_linkAction == 1 ?
			winrt::Windows::UI::Xaml::Visibility::Collapsed :
			winrt::Windows::UI::Xaml::Visibility::Visible;
	}
}
