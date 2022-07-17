#pragma once

#include "ViewModel.RecentViewModel.g.h"

namespace winrt::OsuPlayer::ViewModel::implementation
{
    struct RecentViewModel : RecentViewModelT<RecentViewModel>
    {
        RecentViewModel() = default;

    };
}

namespace winrt::OsuPlayer::ViewModel::factory_implementation
{
    struct RecentViewModel : RecentViewModelT<RecentViewModel, implementation::RecentViewModel>
    {
    };
}
