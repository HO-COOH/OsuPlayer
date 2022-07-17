#pragma once
#include "SongItem.g.h"
#include "ViewModel.CollectionItem.g.h"

namespace winrt::OsuPlayer::ViewModel::implementation
{
    struct CollectionItem : CollectionItemT<CollectionItem>
    {
        CollectionItem() = default;

        winrt::hstring Name();
        void Name(winrt::hstring name);

        Windows::Foundation::Collections::IObservableVector<SongItem> SongItems();

        int Count();
    private:
        winrt::hstring m_name;
        Windows::Foundation::Collections::IObservableVector<SongItem> m_songItems = winrt::single_threaded_observable_vector<OsuPlayer::SongItem>();
    };
}

namespace winrt::OsuPlayer::ViewModel::factory_implementation
{
    struct CollectionItem : CollectionItemT<CollectionItem, implementation::CollectionItem>
    {
    };
}
