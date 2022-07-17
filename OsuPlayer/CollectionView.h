#pragma once

#include "CollectionView.g.h"
#include "ViewModel.CollectionItem.g.h"

namespace winrt::OsuPlayer::implementation
{
    struct CollectionView : CollectionViewT<CollectionView>
    {
        CollectionView();

        Windows::Foundation::Collections::IObservableVector<OsuPlayer::ViewModel::CollectionItem> CollectionItems();
    private:
        Windows::Foundation::Collections::IObservableVector<OsuPlayer::ViewModel::CollectionItem> m_collectionItems;
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct CollectionView : CollectionViewT<CollectionView, implementation::CollectionView>
    {
    };
}
