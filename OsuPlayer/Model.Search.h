#pragma once
#include "Model.SongItem.h"
#include <future>

namespace Model
{
	class SearchModel
	{
	public:
		SearchModel();

		std::future<std::vector<SongItemModel>> doSearch(winrt::hstring keyword);
	private:
		std::vector<SongItemModel> const& m_songs;
	};

}