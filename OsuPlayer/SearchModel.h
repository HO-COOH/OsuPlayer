#pragma once
#include "SongItemModel.h"
#include <future>
class SearchModel
{
public:
	SearchModel();

	std::future<std::vector<SongItemModel>> doSearch(winrt::hstring keyword);
private:
	std::vector<SongItemModel> const& m_songs;
};

