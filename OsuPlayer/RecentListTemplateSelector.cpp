#include "pch.h"
#include "RecentListTemplateSelector.h"
#if __has_include("RecentListTemplateSelector.g.cpp")
#include "RecentListTemplateSelector.g.cpp"
#endif

#include "ViewModel.SongItemViewModel.g.h"

namespace winrt::OsuPlayer::implementation
{
	winrt::Windows::UI::Xaml::DataTemplate RecentListTemplateSelector::SelectTemplateCore(winrt::Windows::Foundation::IInspectable const& object)
	{
		if (object.try_as<ViewModel::SongItemViewModel>())
			return m_songTemplate;
		return m_collectionTemplate;
	}

	winrt::Windows::UI::Xaml::DataTemplate RecentListTemplateSelector::SelectTemplateCore(
		winrt::Windows::Foundation::IInspectable const& object, 
		[[maybe_unused]]winrt::Windows::UI::Xaml::DependencyObject container)
	{
		return SelectTemplateCore(object);
	}
}
