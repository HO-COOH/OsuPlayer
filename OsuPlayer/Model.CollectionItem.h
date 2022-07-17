#pragma once
#include <string>
#include <vector>
#include "OsuDBParser.hpp"

namespace Model
{
	struct CollectionItemModel
	{
		std::string m_name;
		std::vector<Db::Beatmap> m_beatmapPtr;
	};

	std::vector<CollectionItemModel> GetCollectionItemModel(std::unordered_set<Db::Beatmap>&& beatmap, winrt::Windows::Storage::StorageFile file);
}
