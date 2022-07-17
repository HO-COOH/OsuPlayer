#include "pch.h"
#include "ViewModel.Settings.h"
#if __has_include("ViewModel.SettingsViewModel.g.cpp")
#include "ViewModel.SettingsViewModel.g.cpp"
#endif

#include <winrt/Windows.UI.Xaml.Controls.h>
#include "PlayMods.h"
#include "Model.MyMusic.h"

namespace winrt::OsuPlayer::ViewModel::implementation
{
	winrt::Windows::Foundation::IAsyncAction SettingsViewModel::AddOsuPath()
	{
		co_await m_model.doPickOsuFolder();
		m_propertyChanged(*this, winrt::Windows::UI::Xaml::Data::PropertyChangedEventArgs{ L"OsuPaths" });
		co_await Model::MyMusicModel::StartIndexing();

	}

	void SettingsViewModel::ClearAll()
	{
	}

	winrt::event_token SettingsViewModel::PropertyChanged(winrt::Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
	{
		return m_propertyChanged.add(handler);
	}

	void SettingsViewModel::PropertyChanged(winrt::event_token const& token) noexcept
	{
		m_propertyChanged.remove(token);
	}

	winrt::Windows::Foundation::Collections::IObservableVector<winrt::hstring> SettingsViewModel::Skins()
	{
		return winrt::Windows::Foundation::Collections::IObservableVector<winrt::hstring>();
	}

	winrt::Windows::Foundation::Collections::IObservableVector<winrt::OsuPlayer::OsuPathItem> SettingsViewModel::OsuPaths()
	{
		auto const& osuFolders = m_model.OsuFolders();

		//auto osuFolderItems = winrt::
		//for (auto const& osuFolder : osuFolders)
		//{

		//}
		return winrt::Windows::Foundation::Collections::IObservableVector<winrt::OsuPlayer::OsuPathItem>();
	}

	Mod SettingsViewModel::DefaultMod() const
	{
		switch (m_model.Mod())
		{
			case PlayMods::Normal:		return Mod::Normal;
			case PlayMods::HalfTime:	return Mod::HalfTime;
			case PlayMods::DoubleTime:	return Mod::DoubleTime;
			case PlayMods::NightCore:	return Mod::NightCore;
		}
		assert(false); //Mod not found?
	}

	void SettingsViewModel::DefaultMod(Mod mod)
	{
		switch (mod)
		{
			case Mod::Normal:		m_model.Mod(PlayMods::Normal);		break;
			case Mod::HalfTime:	m_model.Mod(PlayMods::HalfTime);	break;
			case Mod::DoubleTime: m_model.Mod(PlayMods::DoubleTime);	break;
			case Mod::NightCore:	m_model.Mod(PlayMods::NightCore);	break;
		}
		assert(false); //Mod not found?
	}
}
