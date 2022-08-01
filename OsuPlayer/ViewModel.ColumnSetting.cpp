#include "pch.h"
#include "ViewModel.ColumnSetting.h"
#if __has_include("ViewModel.ColumnSettingItem.g.cpp")
#include "ViewModel.ColumnSettingItem.g.cpp"
#include "ViewModel.ColumnSettings.g.cpp"
#endif

#include "ViewModelLocator.h"
#include "Utils.h"
#include <winrt/Windows.ApplicationModel.Resources.h>

namespace winrt::OsuPlayer::ViewModel::implementation
{
	struct ResourceName
	{
		constexpr static inline wchar_t const* Singer = L"SingerColumnHeaderText";
		constexpr static inline wchar_t const* Mapper = L"MapperColumnHeaderText";
		constexpr static inline wchar_t const* Length = L"LengthColumnHeaderText";
		constexpr static inline wchar_t const* Version = L"VersionColumnHeaderText";
	};

	void implementation::ColumnSettingItem::Show(bool show)
	{
		if (show != m_show)
		{
			m_show = show;
			raisePropertyChange(L"Show");
			ViewModelLocator::Current().ColumnSettings().UpdateVisibilityButtonStatus();
		}
	}

	winrt::Windows::Storage::ApplicationDataContainer ColumnSettings::m_localSettings = winrt::Windows::Storage::ApplicationData::Current().LocalSettings();

	ColumnSettings::ColumnSettings()
	{
		auto resourceLoader = winrt::Windows::ApplicationModel::Resources::ResourceLoader::GetForCurrentView();
		

		auto itemsString = Utils::SplitPathFromString(winrt::unbox_value_or<winrt::hstring>(m_localSettings.Values().TryLookup(L"Columns"), L""));
		if (itemsString.empty())
		{
			//for (auto const& item : ItemNames)
			//{
			//	m_items.Append(ViewModel::ColumnSettingItem{ true, item });
			//}
			m_items.Append(ViewModel::ColumnSettingItem{ true, resourceLoader.GetString(ResourceName::Singer) });
			m_items.Append(ViewModel::ColumnSettingItem{ true, resourceLoader.GetString(ResourceName::Mapper) });
			m_items.Append(ViewModel::ColumnSettingItem{ true, resourceLoader.GetString(ResourceName::Length) });
			m_items.Append(ViewModel::ColumnSettingItem{ true, resourceLoader.GetString(ResourceName::Version) });
		}
		else
		{
			for (auto const& itemString : itemsString)
			{
				if (itemString.empty())
					continue;
				m_items.Append(ViewModel::ColumnSettingItem{ itemString[0] == L'1', winrt::hstring(&itemString[1])});
			}
		}
	}

	int implementation::ColumnSettings::SelectedIndex()
	{
		return m_selectedIndex;
	}

	void implementation::ColumnSettings::SelectedIndex(int selectedIndex)
	{
		if (selectedIndex == -1)
		{
			return;
		}
		if (selectedIndex != m_selectedIndex)
		{
			m_selectedIndex = selectedIndex;
			raisePropertyChange(L"SelectedIndex");
			raisePropertyChange(L"IsMoveUpEnabled");
			raisePropertyChange(L"IsMoveDownEnabled");
			raisePropertyChange(L"IsShowEnabled");
			raisePropertyChange(L"IsHideEnabled");
		}
	}

	bool implementation::ColumnSettings::IsMoveUpEnabled()
	{
		return m_selectedIndex != 0 && m_selectedIndex != -1;
	}

	bool implementation::ColumnSettings::IsMoveDownEnabled()
	{
		return m_selectedIndex != m_items.Size() - 1 && m_selectedIndex != -1;
	}

	bool implementation::ColumnSettings::IsShowEnabled()
	{
		return m_selectedIndex != -1 && !m_items.GetAt(m_selectedIndex).Show();
	}

	bool implementation::ColumnSettings::IsHideEnabled()
	{
		return m_selectedIndex != -1 && !IsShowEnabled();
	}

	void implementation::ColumnSettings::MoveUpItem()
	{
		assert(m_selectedIndex > 0 && m_selectedIndex < m_items.Size());
		auto const indexCopy = m_selectedIndex;
		auto item = m_items.GetAt(m_selectedIndex);
		m_items.RemoveAt(m_selectedIndex);
		m_items.InsertAt(m_selectedIndex - 1, item);
		SelectedIndex(indexCopy - 1);
	}

	void implementation::ColumnSettings::MoveDownItem()
	{
		assert(m_selectedIndex != -1 && m_selectedIndex < m_items.Size() - 1);
		auto const indexCopy = m_selectedIndex;
		auto item = m_items.GetAt(m_selectedIndex);
		m_items.RemoveAt(m_selectedIndex);
		m_items.InsertAt(m_selectedIndex + 1, item);
		SelectedIndex(indexCopy + 1);
	}

	void implementation::ColumnSettings::ShowItem()
	{
		assertIndexValid();
		m_items.GetAt(m_selectedIndex).Show(true);
		UpdateVisibilityButtonStatus();
	}

	void implementation::ColumnSettings::HideItem()
	{
		assertIndexValid();
		m_items.GetAt(m_selectedIndex).Show(false);
		UpdateVisibilityButtonStatus();
	}

	void implementation::ColumnSettings::UpdateVisibilityButtonStatus()
	{
		raisePropertyChange(L"IsShowEnabled");
		raisePropertyChange(L"IsHideEnabled");
	}

	void implementation::ColumnSettings::Save()
	{
		std::wstring result;
		for (auto const& item : m_items)
		{
			result += (item.Show() ? L'1' : L'0');
			result += item.Name();
			result += L';';
		}
		m_localSettings.Values().Insert(L"Columns", winrt::box_value(winrt::hstring(result)));
	}
}
