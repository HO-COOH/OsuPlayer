#pragma once

#include "winrt/Windows.UI.Xaml.h"
#include <winrt/Windows.Foundation.h>
#include "OsuPathItem.g.h"
#include "ViewModel.OsuPathItemViewModel.g.h"

namespace winrt::OsuPlayer::implementation
{
    struct OsuPathItem : OsuPathItemT<OsuPathItem>
    {
        OsuPathItem();

        //Construct directly with a view model
        OsuPathItem(ViewModel::OsuPathItemViewModel viewModel);

        ViewModel::OsuPathItemViewModel ViewModel();

        void DeleteButton_Click(
            winrt::Windows::Foundation::IInspectable const& sender,
            winrt::Windows::UI::Xaml::RoutedEventArgs const& e
        );
    private:
        ViewModel::OsuPathItemViewModel m_model;
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct OsuPathItem : OsuPathItemT<OsuPathItem, implementation::OsuPathItem>
    {
    };
}
