#pragma once

#include "VolumeToIconConverter.g.h"

namespace winrt::OsuPlayer::implementation
{
    /**
     * @brief This class convert Unnormalized volume (ranging 0~100) -> Icon(represented as a glyph string) 
    */
    struct VolumeToIconConverter : VolumeToIconConverterT<VolumeToIconConverter>
    {
        VolumeToIconConverter() = default;

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
    struct VolumeToIconConverter : VolumeToIconConverterT<VolumeToIconConverter, implementation::VolumeToIconConverter>
    {
    };
}
