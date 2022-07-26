#include "pch.h"
#include "VolumeToIconConverter.h"
#if __has_include("VolumeToIconConverter.g.cpp")
#include "VolumeToIconConverter.g.cpp"
#endif
#include <array>

namespace winrt::OsuPlayer::implementation
{
    winrt::Windows::Foundation::IInspectable VolumeToIconConverter::Convert(
        winrt::Windows::Foundation::IInspectable const& value, 
        [[maybe_unused]]winrt::Windows::UI::Xaml::Interop::TypeName const& targetType, 
        [[maybe_unused]]winrt::Windows::Foundation::IInspectable const& parameter, 
        [[maybe_unused]]winrt::hstring const& language)
    {
        constexpr wchar_t const* MuteGlyph = L"&#xE74F;";
        constexpr std::array<wchar_t const*, 4> VolumeGlyphs
        {
            L"\uE992",
            L"\uE993",
            L"\uE994",
            L"\uE995",
        };
        int const volume = winrt::unbox_value<int>(value);
        if (volume == 0)        
            return winrt::box_value(MuteGlyph);
        else 
            return winrt::box_value(VolumeGlyphs[std::clamp<int>((volume / 25), 0, VolumeGlyphs.size() - 1)]);
    }

    winrt::Windows::Foundation::IInspectable VolumeToIconConverter::ConvertBack(
        [[maybe_unused]]winrt::Windows::Foundation::IInspectable const& value, 
        [[maybe_unused]]winrt::Windows::UI::Xaml::Interop::TypeName const& targetType, 
        [[maybe_unused]]winrt::Windows::Foundation::IInspectable const& parameter, 
        [[maybe_unused]]winrt::hstring const& language)
    {
        return {};
    }
}
