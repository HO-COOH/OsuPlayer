#pragma once

#include "Recent.g.h"

namespace winrt::OsuPlayer::implementation
{
    struct Recent : RecentT<Recent>
    {
        Recent();

        int32_t MyProperty();
        void MyProperty(int32_t value);

        void ClickHandler(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args);
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct Recent : RecentT<Recent, implementation::Recent>
    {
    };
}
