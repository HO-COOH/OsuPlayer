#pragma once
#include "ViewModel.CollectionItem.g.h"

namespace winrt::OsuPlayer::ViewModel::implementation
{
    struct CollectionItem : CollectionItemT<CollectionItem>
    {
        CollectionItem() = default;

        winrt::hstring Name();
        void Name(winrt::hstring name);

        Windows::Foundation::Collections::IObservableVector<ViewModel::SongItemViewModel> SongItems();

        int Count();
    private:
        winrt::hstring m_name;
        Windows::Foundation::Collections::IObservableVector<ViewModel::SongItemViewModel> m_songItems = winrt::single_threaded_observable_vector<OsuPlayer::ViewModel::SongItemViewModel>();
    };
}

namespace winrt::OsuPlayer::ViewModel::factory_implementation
{
    struct CollectionItem : CollectionItemT<CollectionItem, implementation::CollectionItem>
    {
    };
}
