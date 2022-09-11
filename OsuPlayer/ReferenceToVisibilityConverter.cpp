#include "pch.h"
#include "ReferenceToVisibilityConverter.h"
#if __has_include("ReferenceToVisibilityConverter.g.cpp")
#include "ReferenceToVisibilityConverter.g.cpp"
#endif

namespace winrt::OsuPlayer::implementation
{
	winrt::Windows::Foundation::IInspectable ReferenceToVisibilityConverter::Convert(
		winrt::Windows::Foundation::IInspectable const& value, 
		[[maybe_unused]]winrt::Windows::UI::Xaml::Interop::TypeName const& targetType, 
		[[maybe_unused]]winrt::Windows::Foundation::IInspectable const& parameter,
		[[maybe_unused]]winrt::hstring const& language)
	{
		return winrt::box_value(
			value ? winrt::Windows::UI::Xaml::Visibility::Visible : winrt::Windows::UI::Xaml::Visibility::Collapsed
		);
	}

	winrt::Windows::Foundation::IInspectable ReferenceToVisibilityConverter::ConvertBack(
		[[maybe_unused]]winrt::Windows::Foundation::IInspectable const& value, 
		[[maybe_unused]]winrt::Windows::UI::Xaml::Interop::TypeName const& targetType, 
		[[maybe_unused]]winrt::Windows::Foundation::IInspectable const& parameter, 
		[[maybe_unused]]winrt::hstring const& language)
	{
		throw std::exception{ "Not implemented" };
	}
}
