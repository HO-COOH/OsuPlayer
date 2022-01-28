#pragma once
#include "SongItem.g.h"
#include "CollectionItemViewModel.g.h"

namespace winrt::OsuPlayer::implementation
{
    struct CollectionItemViewModel : CollectionItemViewModelT<CollectionItemViewModel>
    {
        CollectionItemViewModel() = default;

        winrt::hstring Name();
        void Name(winrt::hstring name);

        Windows::Foundation::Collections::IObservableVector<OsuPlayer::SongItem> SongItems();

        int Count();
    private:
        winrt::hstring m_name;
        Windows::Foundation::Collections::IObservableVector<OsuPlayer::SongItem> m_songItems = winrt::single_threaded_observable_vector<OsuPlayer::SongItem>();
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct CollectionItemViewModel : CollectionItemViewModelT<CollectionItemViewModel, implementation::CollectionItemViewModel>
    {
    };
}
