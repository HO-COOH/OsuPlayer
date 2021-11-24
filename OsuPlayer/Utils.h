#pragma once

#include <pch.h>
#include <winrt/Windows.UI.Xaml.Media.h>

namespace Utils
{
	template<typename T>
	T FindParent(winrt::Windows::UI::Xaml::DependencyObject const& control)
	{
		auto parent = winrt::Windows::UI::Xaml::Media::VisualTreeHelper::GetParent(control);
		while (parent && parent.try_as<T>())
		{
			parent = winrt::Windows::UI::Xaml::Media::VisualTreeHelper::GetParent(parent);
		}
		return parent.try_as<T>();
	}
}
