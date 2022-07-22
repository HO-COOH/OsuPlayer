#include "pch.h"
#include "Model.CollectionItem.h"
#include "Utils.FileStreamAdaptor.hpp"

namespace Model
{
	std::vector<CollectionItemModel> GetCollectionItemModel(std::unordered_set<Db::Beatmap>&& beatmaps, winrt::Windows::Storage::StorageFile file)
	{
		Utils::StreambufAdaptor buf{ file };
		Db::Collections collectionsDb{ buf.getBuffer() };

		std::vector<CollectionItemModel> result;
		result.reserve(collectionsDb.collections.size());

		for (auto& collection : collectionsDb.collections)
		{
			CollectionItemModel collectionItemModel;
			collectionItemModel.m_name = std::move(collection.name);
			for (auto const& collectionBeatmapMd5 : collection.md5s)
			{
				Db::Beatmap temp;
				temp.md5 = collectionBeatmapMd5;
				if (auto iterBeatmap = beatmaps.find(temp); iterBeatmap != beatmaps.end())
					collectionItemModel.m_beatmapPtr.push_back(std::move(*iterBeatmap));
			}
			result.push_back(std::move(collectionItemModel));
		}
		return result;
	}

	namespace Test
	{
		std::vector<CollectionItemModel> Model::Test::GetCollectionItemModel(std::unordered_map<std::string_view, Db::Beatmap const*> map, winrt::Windows::Storage::StorageFile file)
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

}

