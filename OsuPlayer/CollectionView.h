#pragma once

#include "CollectionView.g.h"

namespace winrt::OsuPlayer::implementation
{
    struct CollectionView : CollectionViewT<CollectionView>
    {
        CollectionView();

        int32_t MyProperty();
        void MyProperty(int32_t value);

        void ClickHandler(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args);
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct CollectionView : CollectionViewT<CollectionView, implementation::CollectionView>
    {
    };
}
