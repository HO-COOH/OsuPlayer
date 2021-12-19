#include "pch.h"
#include "SearchModel.h"
#include <MyMusicModel.h>

SearchModel::SearchModel() : m_songs{MyMusicModel::m_songs}
{
}

std::future<std::vector<SongItemModel>> SearchModel::doSearch(winrt::hstring keyword)
{
	return std::async(
		std::launch::async,
		[keyword, this]()
		{
			std::vector<SongItemModel> result;
			std::copy_if(
				m_songs.cbegin(),
				m_songs.cend(),
				std::back_inserter(result),
				[keyword](SongItemModel const& songItem)
				{
					std::wstring_view songName{ songItem.SongName() };
					std::wstring_view singer{ songItem.Singer() };
					std::wstring_view mapper{ songItem.Mapper() };
					return songName.find(keyword) != std::wstring_view::npos ||
						songName.find(singer) != std::wstring_view::npos ||
						mapper.find(singer) != std::wstring_view::npos;
				}
			);
			return result;
		}
	);
}
