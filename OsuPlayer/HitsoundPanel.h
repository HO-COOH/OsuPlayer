#pragma once

#include "HitsoundPanel.g.h"
#include "ViewModel.HitsoundPanelViewModel.g.h"

namespace winrt::OsuPlayer::implementation
{
    struct HitsoundPanel : HitsoundPanelT<HitsoundPanel>
    {
        HitsoundPanel();

        ViewModel::HitsoundPanelViewModel ViewModel();
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct HitsoundPanel : HitsoundPanelT<HitsoundPanel, implementation::HitsoundPanel>
    {
    };
}
