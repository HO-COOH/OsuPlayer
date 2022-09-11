#include "pch.h"
#include "ViewModel.Recent.h"
#if __has_include("ViewModel.RecentViewModel.g.cpp")
#include "ViewModel.RecentViewModel.g.cpp"
#endif

#include "Model.Recent.h"

namespace winrt::OsuPlayer::ViewModel::implementation
{
	void RecentViewModel::addSongRecord(ViewModel::SongItemViewModel songItem)
	{
		if(auto const removedIndex = Model::Recent::GetInstance().addRecordForSong(*reinterpret_cast<Model::SongItemModel const*>(winrt::unbox_value<size_t>(songItem.ModelPointer())));
			removedIndex == -1)
			m_items.Append(songItem);
		else
		{
			m_items.RemoveAt(removedIndex);
			m_items.InsertAt(0, songItem);
		}
	}

	void RecentViewModel::addCollectionRecord(ViewModel::CollectionItem collectionItem)
	{
		//if (auto const removedIndex = Model::Recent::GetInstance().addRe(*reinterpret_cast<Model::SongItemModel const*>(winrt::unbox_value<size_t>(songItem.ModelPointer())));
		//	removedIndex == -1)
		//	m_items.Append(songItem);
		//else
		//{
		//	m_items.RemoveAt(removedIndex);
		//	m_items.InsertAt(0, songItem);
		//}
	}

}
