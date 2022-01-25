#include "pch.h"
#include "MyMusicModel.h"
#include <algorithm>
#include "Utils.h"
#include <execution>

MyMusicModel::MyMusicModel() : m_osuFolders{SettingsModel::m_osuFolders}
{
}

void MyMusicModel::doSort(SortBy sortMethod)
{
	m_sortBy = sortMethod;
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
	m_sortOrder = order;
	doSort(m_sortBy);
}

MyMusicModel::SortOrder MyMusicModel::getSortOrder() const
{
	return m_sortOrder;
}

bool MyMusicModel::HasFinishedIndexing()
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

void MyMusicModel::sortByArtist()
{
	std::sort(
		std::execution::par_unseq,
		m_songs.begin(),
		m_songs.end(),
		[](SongItemModel const& lhs, SongItemModel const& rhs)
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
		[](SongItemModel const& lhs, SongItemModel const& rhs)
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
		[](SongItemModel const& lhs, SongItemModel const& rhs)
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
		[](SongItemModel const& lhs, SongItemModel const& rhs)
		{
			return m_sortOrder == SortOrder::Ascend ?
				(lhs.SongName() < rhs.SongName()) :
				(lhs.SongName() > rhs.SongName());
		}
	);
}

winrt::Windows::Foundation::IAsyncAction MyMusicModel::StartIndexing()
{
	//Indexing every osu folder if necessary
	for (auto const& osuFolder : SettingsModel::OsuFolders())
	{
		auto songFolders = co_await osuFolder.GetFoldersAsync();
		co_await std::async(
			std::launch::async, 
			[&songFolders] 
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
		auto const& collectionFile = SettingsModel::GetLocalData<SettingsModel::LocalDataType::CollectionDB>();
		auto const& osuDbFile = SettingsModel::GetLocalData<SettingsModel::LocalDataType::OsuDB>();

		assert(collectionFile.size() == osuDbFile.size());
		for (auto i = 0; i < osuDbFile.size(); ++i)
		{
			StreambufAdaptor osuDbBuf{ osuDbFile[i] };
			auto collectionItemModel = GetCollectionItemModel(Db::Osu{ osuDbBuf.getBuffer()}.getBeatmapSet(), collectionFile[i]);
			std::move(collectionItemModel.begin(), collectionItemModel.end(), std::back_inserter(m_collections));
		}
	}


	//Call event handlers
	for (auto& handler : s_handlers)
	{
		co_await winrt::resume_foreground(winrt::Windows::ApplicationModel::Core::CoreApplication::MainView().CoreWindow().Dispatcher());
		handler(m_songs);
	}

}

void MyMusicModel::OnIndexingFinished(std::function<void(std::vector<SongItemModel>const&)> handler)
{
	if (HasFinishedIndexing())
		handler(m_songs);
	s_handlers.emplace_back(std::move(handler));
}

SongItemModel& MyMusicModel::get(int index)
{
	return m_songs[index];
}
