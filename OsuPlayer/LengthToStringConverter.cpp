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
		winrt::Windows::UI::Xaml::Interop::TypeName const& targetType,
		winrt::Windows::Foundation::IInspectable const& parameter, 
		winrt::hstring const& language)
	{
		return winrt::box_value(Utils::GetDurationString(Utils::GetDuration(winrt::unbox_value<int>(value))));
	}

	winrt::Windows::Foundation::IInspectable LengthToStringConverter::ConvertBack(
		winrt::Windows::Foundation::IInspectable const& value, 
		winrt::Windows::UI::Xaml::Interop::TypeName const& targetType, 
		winrt::Windows::Foundation::IInspectable const& parameter, 
		winrt::hstring const& language)
	{
		return winrt::Windows::Foundation::IInspectable();
	}
}
