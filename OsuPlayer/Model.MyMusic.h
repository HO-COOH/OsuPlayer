#pragma once

#include <vector>
#include <winrt/Windows.Storage.h>
#include "Model.SongItem.h"
#include <future>
#include <functional>
#include <array>
#include "Model.CollectionItem.h"

namespace Model
{
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

		

		void setSortOrder(SortOrder order);
		[[nodiscard]] SortOrder getSortOrder() const;

		void setSortby(SortBy method);
		[[nodiscard]] SortBy getSortBy();

		winrt::Windows::Foundation::IAsyncAction startIndexing();

		void onIndexingFinished(std::function<void(std::vector<SongItemModel> const&)> handler);

		 SongItemModel& get(int index);

		[[nodiscard]] bool hasFinishedIndexing();


		std::vector<SongItemModel> m_songs;
		std::vector<CollectionItemModel> m_collections;

		static MyMusicModel& GetInstance();

	private:
		SortOrder m_sortOrder;
		SortBy m_sortBy;

		void sortByArtist();
		void sortByBPM();
		void sortByCreator();
		void sortByDate();
		void sortByDifficulty();
		void sortByLength();
		void sortByRank();
		void sortByTitle();

		void doSort(SortBy sortMethod);
		std::vector<std::future<void>> m_indexingFutures;
		std::vector<std::function<void(std::vector<SongItemModel> const&)>> s_handlers;
	};

}