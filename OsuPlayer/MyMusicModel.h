#pragma once

#include <vector>
#include <winrt/Windows.Storage.h>
#include "SongItemModel.h"
#include <future>
#include "SettingsModel.h"


class MyMusicModel
{
public:
	enum class SortBy
	{
        Artist = 0,
        BPM = 1,
        Creator = 2,
        Date = 3,
        Difficulty = 4,
        Length = 5,
        Rank = 6,
        Title = 7,
	};
	enum class SortOrder
	{
		Ascend,
		Descend
	};
	MyMusicModel();

	void doSort(SortBy sortMethod);

	void setSortOrder(SortOrder order);
	[[nodiscard]] SortOrder getSortOrder() const;

	static winrt::Windows::Foundation::IAsyncAction StartIndexing();
private:

	[[nodiscard]] bool hasFinishedIndexing() const;

	void sortByArtist();
	void sortByBPM();
	void sortByCreator();
	void sortByDate();
	void sortByDifficulty();
	void sortByLength();
	void sortByRank();
	void sortByTitle();

	inline static std::vector<SongItemModel> m_songs;
	inline static SortOrder m_sortOrder;
	inline static SortBy m_sortBy;
	inline static std::vector<std::future<void>> m_indexingFutures;



	std::vector<winrt::Windows::Storage::StorageFolder> const& m_osuFolders;

	friend class SearchModel;
};

