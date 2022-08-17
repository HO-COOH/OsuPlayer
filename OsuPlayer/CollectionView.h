#pragma once

#include "CollectionView.g.h"
#include "ViewModel.CollectionItem.g.h"

namespace winrt::OsuPlayer::implementation
{
    struct CollectionView : CollectionViewT<CollectionView>
    {
        CollectionView();

        winrt::Windows::Foundation::Collections::IObservableVector<ViewModel::CollectionItem> Collections();
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct CollectionView : CollectionViewT<CollectionView, implementation::CollectionView>
    {
    };
}
