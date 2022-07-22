#pragma once
#include <string>
#include <vector>
#include "OsuDBParser.hpp"
#include <string_view>

namespace Model
{
	struct CollectionItemModel
	{
		/**
		 * @brief Name of the collection
		*/
		std::string m_name;

		/**
		 * @brief
		*/
		std::vector<Db::Beatmap> m_beatmapPtr;
	};

	/**
	 * @brief Find collection items from a set of beatmaps
	 * @param beatmap
	 * @param file
	 * @return
	*/
	std::vector<CollectionItemModel> GetCollectionItemModel(std::unordered_set<Db::Beatmap>&& beatmap, winrt::Windows::Storage::StorageFile file);

	namespace Test
	{
		struct CollectionItemModel
		{
			std::string_view m_name;
			std::vector<Db::Beatmap const*> m_beatmapPtr;
		};
		std::vector<CollectionItemModel> GetCollectionItemModel(std::unordered_map<std::string_view, Db::Beatmap const*> map, winrt::Windows::Storage::StorageFile file);

	}
}
