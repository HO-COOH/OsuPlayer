#pragma once

#include "CollectionView.g.h"
#include "CollectionItemViewModel.g.h"

namespace winrt::OsuPlayer::implementation
{
    struct CollectionView : CollectionViewT<CollectionView>
    {
        CollectionView();

        Windows::Foundation::Collections::IObservableVector<OsuPlayer::CollectionItemViewModel> CollectionItems();
    private:
        Windows::Foundation::Collections::IObservableVector<OsuPlayer::CollectionItemViewModel> m_collectionItems;
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct CollectionView : CollectionViewT<CollectionView, implementation::CollectionView>
    {
    };
}
