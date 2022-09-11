#pragma once

#include "RecentPage.g.h"

namespace winrt::OsuPlayer::implementation
{
    struct RecentPage : RecentPageT<RecentPage>
    {
        RecentPage();

        ViewModel::RecentViewModel ViewModel();
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct RecentPage : RecentPageT<RecentPage, implementation::RecentPage>
    {
    };
}
