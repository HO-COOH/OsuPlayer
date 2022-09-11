#pragma once

#include "ViewModel.RecentViewModel.g.h"

namespace winrt::OsuPlayer::ViewModel::implementation
{
    struct RecentViewModel : RecentViewModelT<RecentViewModel>
    {
        RecentViewModel() = default;

        winrt::Windows::Foundation::Collections::IObservableVector<winrt::Windows::Foundation::IInspectable> RecentItems() { return m_items; }

        void addSongRecord(ViewModel::SongItemViewModel songItem);
        void addCollectionRecord(ViewModel::CollectionItem collectionItem);
    private:
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::Windows::Foundation::IInspectable> m_items = winrt::single_threaded_observable_vector<winrt::Windows::Foundation::IInspectable>();
    };
}

namespace winrt::OsuPlayer::ViewModel::factory_implementation
{
    struct RecentViewModel : RecentViewModelT<RecentViewModel, implementation::RecentViewModel>
    {
    };
}
