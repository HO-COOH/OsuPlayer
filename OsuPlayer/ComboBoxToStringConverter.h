#pragma once

#include "ComboBoxToStringConverter.g.h"

namespace winrt::OsuPlayer::implementation
{
    struct ComboBoxToStringConverter : ComboBoxToStringConverterT<ComboBoxToStringConverter>
    {
        ComboBoxToStringConverter() = default;
        
        winrt::Windows::Foundation::IInspectable Convert(
            winrt::Windows::Foundation::IInspectable const& value,
            winrt::Windows::UI::Xaml::Interop::TypeName const& targetType,
            winrt::Windows::Foundation::IInspectable const& parameter,
            winrt::hstring const& language
        );

        winrt::Windows::Foundation::IInspectable ConvertBack(
            winrt::Windows::Foundation::IInspectable const& value,
            winrt::Windows::UI::Xaml::Interop::TypeName const& targetType,
            winrt::Windows::Foundation::IInspectable const& parameter,
            winrt::hstring const& language
        );
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct ComboBoxToStringConverter : ComboBoxToStringConverterT<ComboBoxToStringConverter, implementation::ComboBoxToStringConverter>
    {
    };
}
