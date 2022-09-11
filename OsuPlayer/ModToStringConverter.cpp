#include "pch.h"
#include "ModToStringConverter.h"
#if __has_include("ModToStringConverter.g.cpp")
#include "ModToStringConverter.g.cpp"
#endif

namespace winrt::OsuPlayer::implementation
{
    winrt::Windows::Foundation::IInspectable ModToStringConverter::Convert(
        winrt::Windows::Foundation::IInspectable const& value,
        [[maybe_unused]]winrt::Windows::UI::Xaml::Interop::TypeName const& targetType,
        [[maybe_unused]]winrt::Windows::Foundation::IInspectable const& parameter,
        [[maybe_unused]]winrt::hstring const& language)
    {
        switch (winrt::unbox_value<ViewModel::PlayMod>(value))
        {
            case ViewModel::PlayMod::HalfTime:		    return winrt::box_value(L"HalfTime");
            case ViewModel::PlayMod::DoubleTime:	    return winrt::box_value(L"DoubleTime");
            case ViewModel::PlayMod::NightCore:	        return winrt::box_value(L"NightCore");
            default:					                return winrt::box_value(L"NoMod");
        }
    }

    winrt::Windows::Foundation::IInspectable ModToStringConverter::ConvertBack(
        winrt::Windows::Foundation::IInspectable const& value,
        [[maybe_unused]]winrt::Windows::UI::Xaml::Interop::TypeName const& targetType,
        [[maybe_unused]]winrt::Windows::Foundation::IInspectable const& parameter,
        [[maybe_unused]]winrt::hstring const& language)
    {
        auto const str = winrt::unbox_value<winrt::hstring>(value);
        if (str == L"NoMod")            return winrt::box_value(ViewModel::PlayMod::Normal);
        else if (str == L"HalfTime")    return winrt::box_value(ViewModel::PlayMod::HalfTime);
        else if (str == L"DoubleTime")  return winrt::box_value(ViewModel::PlayMod::DoubleTime);
        else if (str == L"NightCore")   return winrt::box_value(ViewModel::PlayMod::NightCore);
        else assert(false);
    }
}
