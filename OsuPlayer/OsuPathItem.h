#pragma once

#include "winrt/Windows.UI.Xaml.h"
#include <winrt/Windows.Foundation.h>
#include "OsuPathItem.g.h"
#include "OsuPathItemViewModel.g.h"

namespace winrt::OsuPlayer::implementation
{
    struct OsuPathItem : OsuPathItemT<OsuPathItem>
    {
        OsuPathItem();

        //Construct directly with a view model
        OsuPathItem(OsuPlayer::OsuPathItemViewModel viewModel);

        OsuPlayer::OsuPathItemViewModel ViewModel();

        void DeleteButton_Click(
            winrt::Windows::Foundation::IInspectable const& sender,
            winrt::Windows::UI::Xaml::RoutedEventArgs const& e
        );
    private:
        OsuPlayer::OsuPathItemViewModel m_model;
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct OsuPathItem : OsuPathItemT<OsuPathItem, implementation::OsuPathItem>
    {
    };
}
