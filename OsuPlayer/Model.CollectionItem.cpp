#include "pch.h"
#include "Model.CollectionItem.h"
#include "Utils.FileStreamAdaptor.hpp"

namespace Model
{
	std::vector<CollectionItemModel> GetCollectionItemModel(std::unordered_map<std::string_view, Db::Beatmap const*> map, winrt::Windows::Storage::StorageFile file)
	{
		Utils::StreambufAdaptor buf{ file };
		Db::Collections collectionsDb{ buf.getBuffer() };

		std::vector<CollectionItemModel> result;
		result.reserve(collectionsDb.collections.size());

		for (auto& collection : collectionsDb.collections)
		{
			CollectionItemModel collectionItemModel;
			collectionItemModel.m_name = collection.name;
			for (auto const& collectionBeatmapMd5 : collection.md5s)
			{
				if (auto iterBeatmap = map.find(collectionBeatmapMd5); iterBeatmap != map.end())
					collectionItemModel.m_beatmapPtr.push_back(iterBeatmap->second);
			}
			result.push_back(std::move(collectionItemModel));
		}
		return result;
	}
}

