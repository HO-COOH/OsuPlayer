#pragma once

#include "winrt/Windows.UI.Xaml.h"
#include "winrt/Windows.UI.Xaml.Markup.h"
#include "winrt/Windows.UI.Xaml.Interop.h"
#include "winrt/Windows.UI.Xaml.Controls.Primitives.h"
#include "OsuPathItem.g.h"

namespace winrt::OsuPlayer::implementation
{
    struct OsuPathItem : OsuPathItemT<OsuPathItem>
    {
        OsuPathItem();

        bool IsLoading();

        winrt::hstring OsuPathText();
        void OsuPathText(winrt::hstring const& value);

        winrt::hstring BeatmapCountText();

        void DeleteButton_Click(
            winrt::Windows::Foundation::IInspectable const& sender,
            winrt::Windows::UI::Xaml::RoutedEventArgs const& e
        );
    private:
        winrt::hstring m_osuPathText;
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct OsuPathItem : OsuPathItemT<OsuPathItem, implementation::OsuPathItem>
    {
    };
}
