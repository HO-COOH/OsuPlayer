#pragma once

#include "Model.SongItem.h"
#include "Model.CollectionItem.h"
#include <unordered_map>
#include <list>
#include <winrt/Windows.Foundation.h>
#include <variant>
namespace Model
{
	class Recent
	{
		std::list<std::variant<SongItemModel const*, CollectionItemModel const*>> m_recentList;
		std::unordered_map<SongItemModel const*, decltype(m_recentList.begin())> m_recentSongMap;
		std::unordered_map<CollectionItemModel const*, decltype(m_recentList.begin())> m_recentCollectionMap;

		template<typename Map, typename ItemModel>
		[[nodiscard]] int addRecordImpl(ItemModel const& item, Map& mapToFind)
		{
			//the song is in the recent list
			if (auto mapIter = mapToFind.find(&item); mapIter != mapToFind.cend())
			{
				auto const removedIndex = std::distance(m_recentList.begin(), mapIter->second);
				m_recentList.erase(mapIter->second);
				m_recentList.push_front(&item);
				mapIter->second = m_recentList.begin();
				return static_cast<int>(removedIndex);
			}
			else
			{
				m_recentList.push_front(&item);
				mapToFind[&item] = m_recentList.begin();
				return -1;
			}
		}
	public:
		static Recent& GetInstance();

		/**
		 * @brief If this call update the songItem and pushed to the front, return the old index of the item
		 * If this call add a new songItem and pushed to the front, return -1
		 * @param songItem 
		 * @return 
		*/
		[[nodiscard]] int addRecordForSong(SongItemModel const& songItem) 
		{ 
			return addRecordImpl(songItem, m_recentSongMap); 
		}

	
		[[nodiscard]] int addRecordForCollection(CollectionItemModel const& collectionItem) 
		{ 
			return addRecordImpl(collectionItem, m_recentCollectionMap); 
		}

		winrt::fire_and_forget updateJumpList();
	};
}
