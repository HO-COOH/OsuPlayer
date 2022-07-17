#pragma once

#include "ViewModel.MainViewModel.g.h"

namespace winrt::OsuPlayer::ViewModel::implementation
{
    struct MainViewModel : MainViewModelT<MainViewModel>
    {
        MainViewModel() = default;

    };
}

namespace winrt::OsuPlayer::ViewModel::factory_implementation
{
    struct MainViewModel : MainViewModelT<MainViewModel, implementation::MainViewModel>
    {
    };
}
