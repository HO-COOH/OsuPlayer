#include "pch.h"
#include "CollectionItemModel.h"

std::vector<CollectionItemModel> GetCollectionItemModel(std::unordered_set<Db::Beatmap>&& beatmaps, winrt::Windows::Storage::StorageFile file)
{
	StreambufAdaptor buf{ file };
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