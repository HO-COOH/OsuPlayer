#include "pch.h"
#include "ComboBoxToStringConverter.h"
#if __has_include("ComboBoxToStringConverter.g.cpp")
#include "ComboBoxToStringConverter.g.cpp"
#endif

#include <winrt/Windows.UI.Xaml.Controls.h>

namespace winrt::OsuPlayer::implementation
{

    winrt::Windows::Foundation::IInspectable ComboBoxToStringConverter::Convert(
        winrt::Windows::Foundation::IInspectable const& value, 
        [[maybe_unused]]winrt::Windows::UI::Xaml::Interop::TypeName const& targetType, 
        [[maybe_unused]]winrt::Windows::Foundation::IInspectable const& parameter, 
        [[maybe_unused]]winrt::hstring const& language)
    {
        if (auto comboBoxItem = value.try_as<winrt::Windows::UI::Xaml::Controls::ComboBoxItem>(); comboBoxItem)
            return comboBoxItem.Content();
        return winrt::box_value(L"");
    }
    winrt::Windows::Foundation::IInspectable ComboBoxToStringConverter::ConvertBack(
        [[maybe_unused]]winrt::Windows::Foundation::IInspectable const& value, 
        [[maybe_unused]]winrt::Windows::UI::Xaml::Interop::TypeName const& targetType, 
        [[maybe_unused]]winrt::Windows::Foundation::IInspectable const& parameter, 
        [[maybe_unused]]winrt::hstring const& language)
    {
        return {};
    }
}
