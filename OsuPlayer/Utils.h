#pragma once

#include <pch.h>
#include <winrt/Windows.UI.Xaml.Media.h>
#include <utility>
#include <chrono>

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
	
	/**
	 * @brief Get list of skin folders
	 * @param skinFolder Should be the osu "Skins\" folder
	 * @return 
	 */
	auto inline GetSkins(winrt::Windows::Storage::StorageFolder skinFolder)
	{
		//skinFolder.GetFoldersAsync();
	}
}
