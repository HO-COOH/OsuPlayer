#pragma once

#include "RecentViewModel.g.h"

namespace winrt::OsuPlayer::implementation
{
    struct RecentViewModel : RecentViewModelT<RecentViewModel>
    {
        RecentViewModel() = default;

        int32_t MyProperty();
        void MyProperty(int32_t value);
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct RecentViewModel : RecentViewModelT<RecentViewModel, implementation::RecentViewModel>
    {
    };
}
