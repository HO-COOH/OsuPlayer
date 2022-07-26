#include "pch.h"
#include "LengthToStringConverter.h"
#if __has_include("LengthToStringConverter.g.cpp")
#include "LengthToStringConverter.g.cpp"
#endif
#include "Utils.h"

namespace winrt::OsuPlayer::implementation
{
	winrt::Windows::Foundation::IInspectable LengthToStringConverter::Convert(
		winrt::Windows::Foundation::IInspectable const& value, 
		[[maybe_unused]]winrt::Windows::UI::Xaml::Interop::TypeName const& targetType,
		[[maybe_unused]]winrt::Windows::Foundation::IInspectable const& parameter, 
		[[maybe_unused]]winrt::hstring const& language)
	{
		return winrt::box_value(Utils::GetDurationString(Utils::GetDuration(winrt::unbox_value<int>(value))));
	}

	winrt::Windows::Foundation::IInspectable LengthToStringConverter::ConvertBack(
		[[maybe_unused]]winrt::Windows::Foundation::IInspectable const& value, 
		[[maybe_unused]]winrt::Windows::UI::Xaml::Interop::TypeName const& targetType, 
		[[maybe_unused]]winrt::Windows::Foundation::IInspectable const& parameter, 
		[[maybe_unused]]winrt::hstring const& language)
	{
		return winrt::Windows::Foundation::IInspectable();
	}
}
