#pragma once

#include <vector>
#include <winrt/Windows.Storage.h>
#include "SongItemModel.h"
#include <future>
#include "SettingsModel.h"
#include <functional>
#include <array>


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

	static constexpr std::array SortByMethodIndex
	{
		SortBy::Artist,
		SortBy::BPM,
		SortBy::Creator,
		SortBy::Date,
		SortBy::Difficulty,
		SortBy::Length,
		SortBy::Rank,
		SortBy::Title,
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

	static void OnIndexingFinished(std::function<void(std::vector<SongItemModel> const&)> handler);

	static SongItemModel& get(int index);

	[[nodiscard]] static bool HasFinishedIndexing();

	inline static SortOrder m_sortOrder;
	inline static SortBy m_sortBy;
private:

	

	void sortByArtist();
	void sortByBPM();
	void sortByCreator();
	void sortByDate();
	void sortByDifficulty();
	void sortByLength();
	void sortByRank();
	void sortByTitle();

	inline static std::vector<SongItemModel> m_songs;

	inline static std::vector<std::future<void>> m_indexingFutures;

	inline static std::vector<std::function<void(std::vector<SongItemModel> const&)>> s_handlers;

	std::vector<winrt::Windows::Storage::StorageFolder> const& m_osuFolders;

	friend class SearchModel;
};

