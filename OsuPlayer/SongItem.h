﻿#pragma once

#include "winrt/Windows.UI.Xaml.h"
#include "winrt/Windows.UI.Xaml.Markup.h"
#include "winrt/Windows.UI.Xaml.Interop.h"
#include "winrt/Windows.UI.Xaml.Controls.Primitives.h"
#include "SongItem.g.h"

namespace winrt::OsuPlayer::implementation
{
    struct SongItem : SongItemT<SongItem>
    {
        SongItem();

        winrt::hstring SongName() { return {}; }
        winrt::hstring Singer() { return {}; }
        winrt::hstring Mapper() { return {}; }
        winrt::hstring Length() { return L"0:0"; }

        void CheckBox_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void CheckBox_Unchecked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct SongItem : SongItemT<SongItem, implementation::SongItem>
    {
    };
}