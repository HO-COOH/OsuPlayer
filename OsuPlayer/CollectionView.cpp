#include "pch.h"
#include "CollectionView.h"
#if __has_include("CollectionView.g.cpp")
#include "CollectionView.g.cpp"
#endif
#include <winrt/Windows.Foundation.Collections.h>
#include "Model.MyMusic.h"

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::OsuPlayer::implementation
{
    CollectionView::CollectionView()
    {
        InitializeComponent();
       
        m_collectionItems = winrt::single_threaded_observable_vector<OsuPlayer::ViewModel::CollectionItem>();

        for (auto const& collectionItemModel : Model::MyMusicModel::GetInstance().m_collections)
        {
            ViewModel::CollectionItem collectionItemViewModel;
            collectionItemViewModel.Name(winrt::to_hstring(collectionItemModel.m_name));

            for (auto const& songItem : collectionItemModel.m_beatmapPtr)
            {
                ViewModel::SongItemViewModel itemViewModel;
                //itemViewModel.SongName(winrt::to_hstring(songItem.songTitle));
                //itemViewModel.Singer(winrt::to_hstring(songItem.artistName));
                //itemViewModel.Mapper(winrt::to_hstring(songItem.creator));
                //itemViewModel.Length(songItem.totalTime);

                itemViewModel.SongName(winrt::to_hstring(songItem->songTitle));
                itemViewModel.Singer(winrt::to_hstring(songItem->artistName));
                itemViewModel.Mapper(winrt::to_hstring(songItem->creator));
                itemViewModel.Length(songItem->totalTime);

                collectionItemViewModel.SongItems().Append(itemViewModel);
            }

            m_collectionItems.Append(collectionItemViewModel);
        }

    }

    Windows::Foundation::Collections::IObservableVector<OsuPlayer::ViewModel::CollectionItem> CollectionView::CollectionItems()
    {
        return m_collectionItems;
    }

}
