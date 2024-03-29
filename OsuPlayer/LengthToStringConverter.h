﻿#pragma once

#include "LengthToStringConverter.g.h"

namespace winrt::OsuPlayer::implementation
{
    /**
     * @brief This class convert milliseconds to a string in "<min>:<sec>" format
     * @details Used in the Player control and various other places which needs a
     * string representation of the length of a song
    */
    struct LengthToStringConverter : LengthToStringConverterT<LengthToStringConverter>
    {
        LengthToStringConverter() = default;

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
    struct LengthToStringConverter : LengthToStringConverterT<LengthToStringConverter, implementation::LengthToStringConverter>
    {
    };
}
