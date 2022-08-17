#pragma once

#include <pch.h>
#include <winrt/Windows.UI.Xaml.Media.h>
#include <utility>
#include <chrono>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.UI.Notifications.h>
#include <winrt/Windows.Data.Xml.Dom.h>
#include <winrt/Windows.UI.Core.h>

namespace Utils
{
	template<typename T>
	T FindParent(winrt::Windows::UI::Xaml::DependencyObject const& control)
	{
		auto parent = winrt::Windows::UI::Xaml::Media::VisualTreeHelper::GetParent(control);
		while (parent && !parent.try_as<T>())
		{
			parent = winrt::Windows::UI::Xaml::Media::VisualTreeHelper::GetParent(parent);
		}
		return parent.try_as<T>();
	}

	/**
	 * @brief Transform a `std::chrono::duration` to a <min:sec> pair
	 * @param duration A `std::chrono::duration` variable
	 * @tparam ChronoDuration A `std::chrono::duration` type
	 * @return A pair of duration in <min:sec> pair
	 */
	template<typename ChronoDuration>
	std::pair<int, int> GetDuration(std::chrono::duration<ChronoDuration> duration)
	{
		auto const seconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
		auto const minutes = seconds / 60;
		auto const secondsRest = seconds % 60;
		return std::pair{ minutes, secondsRest };
	}

	/**
	 * @brief Transform milliseconds to a <min:sec> pair
	 * @param milliseconds A duration represented in milliseconds
	 * @tparam Type of milliseconds, which should be a arithmetic type
	 * @return A pair of duration in <min:sec> pair
	 */
	template<typename T>
	std::pair<int, int> GetDuration(T milliseconds)
	{
		auto const seconds = milliseconds / 1000;
		auto const minutes = seconds / 60;
		auto const secondsRest = seconds % 60;
		return std::pair{ minutes, secondsRest };
	}

	/**
	 * @brief Transform `<min:sec>` pair to a string presentation
	 * @param duration Should be `<min, sec>` pair
	 * @return `<min>:<sec>` in string
	 */
	inline winrt::hstring GetDurationString(std::pair<int, int> duration)
	{
		auto [min, sec] = duration;
		return 
			winrt::to_hstring(min) + 
			(sec <10 ? L":0" : L":") +
			winrt::to_hstring(sec);
	}

	inline winrt::Windows::Foundation::IAsyncAction ReturnToUIThread()
	{
		co_await winrt::resume_foreground(winrt::Windows::ApplicationModel::Core::CoreApplication::MainView().CoreWindow().Dispatcher());
	}
	
	/**
	 * @brief Get list of skin folders
	 * @param skinFolder Should be the osu "Skins\" folder
	 * @return 
	 */
	auto inline GetSkins(winrt::Windows::Storage::StorageFolder skinFolder)
	{
		//skinFolder.GetFoldersAsync();
	}

	/**
	 * @brief Split a path string that's separated by ;
	*/
	inline std::vector<winrt::hstring> SplitPathFromString(winrt::hstring pathString)
	{
		std::vector<winrt::hstring> result;
		auto iter = pathString.begin();
		auto start = iter;
		while (iter != pathString.end())
		{
			if (*iter == L';')
			{
				result.emplace_back(&(*start), std::distance(start, iter));
				start = iter + 1;
			}
			++iter;
		}
		return result;
	}


	template<typename T>
	void PushIfNotNull(std::vector<std::remove_cv_t<T>>& vec, T&& object)
	{
		if (object)
			vec.push_back(std::forward<T>(object));
	}

	inline auto GetSettings()
	{
		static auto settings = winrt::Windows::Storage::ApplicationData::Current().LocalSettings();
		return settings;
	}

	inline auto SetBadge(int value)
	{
		auto xml = winrt::Windows::UI::Notifications::BadgeUpdateManager::GetTemplateContent(winrt::Windows::UI::Notifications::BadgeTemplateType::BadgeNumber);
		xml.SelectSingleNode(L"/badge")
			.as<winrt::Windows::Data::Xml::Dom::XmlElement>()
			.SetAttribute(L"value", winrt::to_hstring(value));
		winrt::Windows::UI::Notifications::BadgeNotification badge{ xml };
		winrt::Windows::UI::Notifications::BadgeUpdateManager::CreateBadgeUpdaterForApplication().Update(badge);
	}
}