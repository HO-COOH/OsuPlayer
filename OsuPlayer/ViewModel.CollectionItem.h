#pragma once
#include "ViewModel.CollectionItem.g.h"
#include "Model.CollectionItem.h"

namespace winrt::OsuPlayer::ViewModel::implementation
{
    struct CollectionItem : CollectionItemT<CollectionItem>
    {
        CollectionItem() = default;

        winrt::hstring Name();

        Windows::Foundation::Collections::IObservableVector<ViewModel::SongItemViewModel> SongItems();

        winrt::Windows::Foundation::IInspectable ModelPointer();
        void ModelPointer(winrt::Windows::Foundation::IInspectable modelPointer);

        int Count();
    private:
        Model::CollectionItemModel* m_modelPointer{};
        Windows::Foundation::Collections::IObservableVector<ViewModel::SongItemViewModel> m_songItems = winrt::single_threaded_observable_vector<OsuPlayer::ViewModel::SongItemViewModel>();
    };
}

namespace winrt::OsuPlayer::ViewModel::factory_implementation
{
    struct CollectionItem : CollectionItemT<CollectionItem, implementation::CollectionItem>
    {
    };
}
