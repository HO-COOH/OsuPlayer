#pragma once

#include "CollectionView.g.h"
#include "ViewModel.MyMusicViewModel.g.h"

namespace winrt::OsuPlayer::implementation
{
    struct CollectionView : CollectionViewT<CollectionView>
    {
        CollectionView();

        ViewModel::MyMusicViewModel ViewModel();
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct CollectionView : CollectionViewT<CollectionView, implementation::CollectionView>
    {
    };
}
