#pragma once
#include <string>
#include <vector>
#include "OsuDBParser.hpp"
#include <string_view>
#include "Model.SongItem.h"


namespace Model
{
	struct CollectionItemModel
	{
		/**
		 * @brief Name of the collection
		*/
		std::string m_name;

		/**
		 * @brief Pointers to beatmaps that this collection contains
		*/
		std::vector<Db::Beatmap const*> m_beatmapPtr;

		std::vector<Model::SongItemModel const*> m_songItemPtr;
	};

	/**
	 * @brief Find collection items from a set of beatmaps
	 * @param map A map of <beatmapMd5 (string), beatmapPointer>
	 * @param file The file path of `collections.db`
	 * @return A vector of CollectionItem
	 * @details
	 *		A collection in osu's `collections.db` only records its name and an array of beatmap's md5. 
			See the definitions of `OsuDBParser.hpp Db::Collection`
	*/
	std::vector<CollectionItemModel> GetCollectionItemModel(
		std::unordered_map<std::string_view, Db::Beatmap const*> map, 
		winrt::Windows::Storage::StorageFile file
	);


}