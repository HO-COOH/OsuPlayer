#pragma once

#include "Model.SongItem.h"
#include <unordered_map>
#include <list>
#include <winrt/Windows.Foundation.h>
namespace Model
{
	class Recent
	{
		std::list<SongItemModel const*> m_recentList;
		std::unordered_map<SongItemModel const*, decltype(m_recentList.begin())> m_recentMap;
	public:
		static Recent& GetInstance();

		void addRecordForSong(SongItemModel const& songItem);
		winrt::fire_and_forget updateJumpList();
	};
}
