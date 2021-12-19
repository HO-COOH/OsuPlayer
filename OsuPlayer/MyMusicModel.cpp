#include "pch.h"
#include "MyMusicModel.h"
#include <algorithm>

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

bool MyMusicModel::hasFinishedIndexing() const
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
}

void MyMusicModel::sortByBPM()
{
}

void MyMusicModel::sortByCreator()
{
}

void MyMusicModel::sortByDate()
{
}

void MyMusicModel::sortByDifficulty()
{
}

void MyMusicModel::sortByLength()
{
}

void MyMusicModel::sortByRank()
{
}

void MyMusicModel::sortByTitle()
{
}
