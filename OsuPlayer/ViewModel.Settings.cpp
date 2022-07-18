#include "pch.h"
#include "ViewModel.Settings.h"
#if __has_include("ViewModel.SettingsViewModel.g.cpp")
#include "ViewModel.SettingsViewModel.g.cpp"
#endif

#include <winrt/Windows.UI.Xaml.Controls.h>
#include "PlayMods.h"
#include "Model.MyMusic.h"
#include "Utils.h"

namespace winrt::OsuPlayer::ViewModel::implementation
{
	winrt::Windows::Storage::ApplicationDataContainer SettingsViewModel::m_localSettings = winrt::Windows::Storage::ApplicationData::Current().LocalSettings();

	SettingsViewModel::SettingsViewModel() :
		m_theme(winrt::unbox_value_or<int>(m_localSettings.Values().TryLookup(L"Theme"), 0)),
		m_mod(static_cast<Mod>(winrt::unbox_value_or<int>(m_localSettings.Values().TryLookup(L"Mod"), 0))),
		m_jumplistRecentSongs(winrt::unbox_value_or<int>(m_localSettings.Values().TryLookup(L"RecentSongs"), 0)),
		m_jumplistRecentCollections(winrt::unbox_value_or<int>(m_localSettings.Values().TryLookup(L"RecentCollections"), 0)),
		m_allowModifyOsuData(winrt::unbox_value_or<bool>(m_localSettings.Values().TryLookup(L"AllowModifyOsuData"), false))
	{
	}

	winrt::Windows::Foundation::IAsyncOperation<AddOsuFolderResult> SettingsViewModel::AddOsuPath()
	{
		throw std::exception{ "Not implemented" };
		//co_await m_model.doPickOsuFolder();
		//raisePropertyChange(L"OsuPaths");
		//co_await Model::MyMusicModel::StartIndexing();
		co_return AddOsuFolderResult::Success;
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
		}
	}

	bool implementation::SettingsViewModel::IsModEnabled()
	{
		return m_mod != Mod::Normal;
	}

	void implementation::SettingsViewModel::IsModEnabled(bool enable)
	{
		if (!enable)
			m_mod = Mod::Normal;
		m_localSettings.Values().Insert(L"Mod", winrt::box_value(static_cast<int>(enable? m_mod : Mod::Normal)));
	}

	winrt::Windows::Foundation::Collections::IObservableVector<winrt::hstring> SettingsViewModel::Skins()
	{
		return winrt::Windows::Foundation::Collections::IObservableVector<winrt::hstring>();
	}

	winrt::Windows::Foundation::Collections::IObservableVector<winrt::OsuPlayer::OsuPathItem> SettingsViewModel::OsuPaths()
	{
		auto paths = winrt::single_threaded_observable_vector<winrt::OsuPlayer::OsuPathItem>();
		auto pathsValue = m_localSettings.Values().TryLookup(L"Paths");
		if (pathsValue)
		{
			for (auto path : Utils::SplitPathFromString(pathsValue.as<winrt::hstring>()))
			{
				winrt::OsuPlayer::ViewModel::OsuPathItemViewModel pathItem;
				pathItem.Path(path);
				paths.Append(winrt::OsuPlayer::OsuPathItem{ pathItem });
			}
		}
		return paths;
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
		}
	}
}
