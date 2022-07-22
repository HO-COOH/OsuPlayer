#define NOMINMAX
#include "pch.h"
#include "Model.SongItem.h"
#include "OsuParser.hpp"
#include <chrono>
#include <winrt/Windows.Storage.FileProperties.h>
#include <algorithm>
#include "Utils.FileStreamAdaptor.hpp"

#include <winrt/Windows.Media.Playback.h>
#include <winrt/Windows.Media.MediaProperties.h>
#include "Model.Folders.h"
#include "Model.MyMusic.h"
#include "Utils.Log.hpp"

namespace Model
{

	winrt::hstring SongItemModel::SongName() const
	{
		auto iter = std::ranges::find_if(m_beatmaps, [](BeatmapInfo const& beatmap) {return !beatmap.beatmapPtr->songTitle.empty(); });
		return iter == m_beatmaps.end() ? L"" : winrt::to_hstring(iter->beatmapPtr->songTitle);
	}

	winrt::hstring SongItemModel::Singer() const
	{
		auto iter = std::ranges::find_if(m_beatmaps, [](BeatmapInfo const& beatmap) {return !beatmap.beatmapPtr->artistName.empty(); });
		return iter == m_beatmaps.end() ? L"" : winrt::to_hstring(iter->beatmapPtr->artistName);
	}

	winrt::hstring SongItemModel::Mapper() const
	{
		auto iter = std::ranges::find_if(m_beatmaps, [](BeatmapInfo const& beatmap) {return !beatmap.beatmapPtr->creator.empty(); });
		return iter == m_beatmaps.end() ? L"" : winrt::to_hstring(iter->beatmapPtr->creator);
	}

	int SongItemModel::Length() const
	{
		return m_length;
	}

	winrt::Windows::Media::Core::MediaSource SongItemModel::Source() const
	{
		return m_songSource;
	}

	winrt::Windows::Foundation::DateTime SongItemModel::DateModified() const
	{
		return std::max_element(
			m_beatmaps.cbegin(),
			m_beatmaps.cend(),
			[](BeatmapInfo const& lhs, BeatmapInfo const& rhs)
			{
				return lhs.dateModified < rhs.dateModified;
			}
		)->dateModified;
	}

	winrt::Windows::Foundation::DateTime SongItemModel::DateCreated() const
	{
		return std::min_element(
			m_beatmaps.cbegin(),
			m_beatmaps.cend(),
			[](BeatmapInfo const& lhs, BeatmapInfo const& rhs)
			{
				return lhs.dateCreated < rhs.dateCreated;
			}
		)->dateCreated;
	}

	std::string SongItemModel::Tags(int versionIndex)
	{
		std::string s;
		for (auto const& tag : getMetadata(versionIndex).tags)
			(s += tag) += " ";
		return s;
	}

	int SongItemModel::BitRate() const
	{
		return m_bitrate;
	}


	void SongItemModel::handleImageFile(winrt::Windows::Storage::StorageFile&& file)
	{

	}

	Metadata& SongItemModel::getMetadata(int index)
	{
		if (auto& originalFile = m_beatmaps[index].originalFile; originalFile.has_value())
			return originalFile->metaData;


		Utils::StreambufAdaptor buf{ getFileOf(m_beatmaps[index]).get()};
		m_beatmaps[index].originalFile.emplace(std::istream{ &buf });
		return m_beatmaps[index].originalFile->metaData;
	}


	winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Storage::StorageFile> SongItemModel::getFileOf(BeatmapInfo const& info)
	{
		if (m_folder)
		{
			auto file = co_await m_folder.GetFileAsync(winrt::to_hstring(info.beatmapPtr->osuFileName));
			co_return file;
		}
		co_return winrt::Windows::Storage::StorageFile{nullptr};
	}

	winrt::Windows::Foundation::IAsyncAction SongItemModel::fillDataAsync()
	{
			for (auto const& beatmap : m_beatmaps)
			{
				try
				{
					if (m_bitrate != 0 && m_length != 0)
						break;

					if (beatmap.beatmapPtr->folderName.empty())
						continue;
					winrt::Windows::Storage::StorageFolder osuSongFolder = MyMusicModel::GetInstance().getCurrentIndexingFolder();
					auto thisFolder = co_await osuSongFolder.GetFolderAsync(winrt::to_hstring(beatmap.beatmapPtr->folderName.data()));
					if (thisFolder)
					{
						m_folder = thisFolder;
					}

					if (!m_songSource)
					{
						auto sourceFile = co_await m_folder.GetFileAsync(winrt::to_hstring(beatmap.beatmapPtr->audioFileName));
						if (!sourceFile)
						{
							Utils::ConsoleLogger{} << beatmap.beatmapPtr->osuFileName.data() << " (X)\n";
							continue;
						}
						m_songSource = winrt::Windows::Media::Core::MediaSource::CreateFromStorageFile(sourceFile);
					}

					co_await m_songSource.OpenAsync();
					if (m_songSource.IsOpen())
					{
						winrt::Windows::Media::Playback::MediaPlaybackItem item{ m_songSource };

						if (m_bitrate == 0)
						{
							m_bitrate = item.AudioTracks().GetAt(0).GetEncodingProperties().Bitrate();
						}

						if (m_length == 0)
						{
							m_length = (std::chrono::duration_cast<std::chrono::milliseconds>(m_songSource.Duration().Value())).count();
						}
						m_songSource.Close();
					}
				}
				catch (...)
				{
				}
			}
	}

}