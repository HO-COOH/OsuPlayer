#pragma once

#include "ReferenceToVisibilityConverter.g.h"

namespace winrt::OsuPlayer::implementation
{
    /**
     * @brief This class convert WinRT reference -> Visibility
     * @details null -> Collapsed, otherwise Visible
    */
    struct ReferenceToVisibilityConverter : ReferenceToVisibilityConverterT<ReferenceToVisibilityConverter>
    {
        ReferenceToVisibilityConverter() = default;

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
    struct ReferenceToVisibilityConverter : ReferenceToVisibilityConverterT<ReferenceToVisibilityConverter, implementation::ReferenceToVisibilityConverter>
    {
    };
}
