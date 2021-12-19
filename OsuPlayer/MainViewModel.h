#pragma once

#include "MainViewModel.g.h"

namespace winrt::OsuPlayer::implementation
{
    struct MainViewModel : MainViewModelT<MainViewModel>
    {
        MainViewModel() = default;

    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct MainViewModel : MainViewModelT<MainViewModel, implementation::MainViewModel>
    {
    };
}
