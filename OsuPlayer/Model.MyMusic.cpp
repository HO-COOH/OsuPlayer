#include "pch.h"
#include "Model.MyMusic.h"
#include <algorithm>
#include "Utils.FileStreamAdaptor.hpp"
#include <execution>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <pplawait.h>
#include <ppltasks.h>
#include "Model.Folders.h"

namespace Model
{

	void MyMusicModel::doSort(SortBy sortMethod)
	{
		switch (sortMethod)
		{
			case MyMusicModel::SortBy::Artist: sortByArtist(); break;
			case MyMusicModel::SortBy::BPM: sortByBPM(); break;
			case MyMusicModel::SortBy::Creator: sortByCreator(); break;
			case MyMusicModel::SortBy::Date: sortByDate(); break;
			case MyMusicModel::SortBy::Difficulty: sortByDifficulty(); break;
			case MyMusicModel::SortBy::Length: sortByLength(); break;
			case MyMusicModel::SortBy::Rank: sortByRank(); break;
			case MyMusicModel::SortBy::Title: sortByTitle(); break;
			default: assert(false);	//not supported sorting method
		}
	}

	void MyMusicModel::setSortOrder(SortOrder order)
	{
		if (order == m_sortOrder)
			return;
		m_sortOrder = order;
		doSort(m_sortBy);
	}

	MyMusicModel::SortOrder MyMusicModel::getSortOrder() const
	{
		return m_sortOrder;
	}

	void MyMusicModel::setSortby(SortBy method)
	{
		if (m_sortBy == method)
			return;

		m_sortBy = method;
		doSort(m_sortBy);
	}

	Model::MyMusicModel::SortBy MyMusicModel::getSortBy()
	{
		return m_sortBy;
	}

	bool MyMusicModel::hasFinishedIndexing()
	{
		return std::all_of(
			m_indexingFutures.cbegin(),
			m_indexingFutures.cend(),
			[](std::future<void> const& future)
			{
				return future.wait_for(std::chrono::seconds{ 0 }) == std::future_status::ready;
			}
		);
	}

	MyMusicModel& MyMusicModel::GetInstance()
	{
		static MyMusicModel myMusicModel;
		return myMusicModel;
	}

	void MyMusicModel::sortByArtist()
	{
		std::sort(
			std::execution::par_unseq,
			m_songs.begin(),
			m_songs.end(),
			[this](SongItemModel const& lhs, SongItemModel const& rhs)
			{
				return m_sortOrder == SortOrder::Ascend ?
					(lhs.Singer() < rhs.Singer()) :
					(lhs.Singer() > rhs.Singer());
			}
		);
	}

	void MyMusicModel::sortByBPM()
	{

	}

	void MyMusicModel::sortByCreator()
	{
		std::sort(
			std::execution::par_unseq,
			m_songs.begin(),
			m_songs.end(),
			[this](SongItemModel const& lhs, SongItemModel const& rhs)
			{
				return m_sortOrder == SortOrder::Ascend ?
					(lhs.Mapper() < rhs.Mapper()) :
					(lhs.Mapper() > rhs.Mapper());
			}
		);
	}

	void MyMusicModel::sortByDate()
	{
	}

	void MyMusicModel::sortByDifficulty()
	{
	}

	void MyMusicModel::sortByLength()
	{
		std::sort(
			std::execution::par_unseq,
			m_songs.begin(),
			m_songs.end(),
			[this](SongItemModel const& lhs, SongItemModel const& rhs)
			{
				return m_sortOrder == SortOrder::Ascend ?
					(lhs.Length() < rhs.Length()) :
					(lhs.Length() > rhs.Length());
			}
		);
	}

	void MyMusicModel::sortByRank()
	{
	}

	void MyMusicModel::sortByTitle()
	{
		std::sort(
			std::execution::par_unseq,
			m_songs.begin(),
			m_songs.end(),
			[this](SongItemModel const& lhs, SongItemModel const& rhs)
			{
				return m_sortOrder == SortOrder::Ascend ?
					(lhs.SongName() < rhs.SongName()) :
					(lhs.SongName() > rhs.SongName());
			}
		);
	}

	winrt::Windows::Foundation::IAsyncAction MyMusicModel::startIndexing()
	{
		//Indexing every osu folder if necessary
		for (auto const& osuFolder : Folders::GetInstance().m_songFolders)
		{
			auto songFolders = co_await osuFolder.GetFoldersAsync();
			co_await concurrency::create_task(
				[&songFolders, this]
				{
					std::transform(
						songFolders.begin(),
						songFolders.end(),
						std::back_inserter(m_songs),
						[](winrt::Windows::Storage::StorageFolder const& songFolder)
						{
							return SongItemModel{ songFolder };
						}
					);
				}
				);
		}

		//handle collections
		{
			auto const& collectionFile = Folders::GetInstance().m_collectionDbs;
			auto const& osuDbFile = Folders::GetInstance().m_osuDbs;

			assert(collectionFile.size() == osuDbFile.size());
			for (auto i = 0; i < osuDbFile.size(); ++i)
			{
				Utils::StreambufAdaptor osuDbBuf{ osuDbFile[i] };
				auto collections = GetCollectionItemModel(Db::Osu{ osuDbBuf.getBuffer() }.getBeatmapSet(), collectionFile[i]);
				std::move(collections.begin(), collections.end(), std::back_inserter(m_collections));
			}
		}


		//Call event handlers
		for (auto& handler : s_handlers)
		{
			co_await winrt::resume_foreground(winrt::Windows::ApplicationModel::Core::CoreApplication::MainView().CoreWindow().Dispatcher());
			handler(m_songs);
		}

	}

	void MyMusicModel::onIndexingFinished(std::function<void(std::vector<SongItemModel>const&)> handler)
	{
		if (hasFinishedIndexing())
			handler(m_songs);
		s_handlers.emplace_back(std::move(handler));
	}

	SongItemModel& MyMusicModel::get(int index)
	{
		return m_songs[index];
	}
}
