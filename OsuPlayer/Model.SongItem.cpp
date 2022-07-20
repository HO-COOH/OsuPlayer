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

namespace Model
{
	std::unordered_map<winrt::hstring, SongItemModel::FileHandlerFunction> SongItemModel::fileHandlers
	{
		{L".osu", &SongItemModel::handleOsuFile},
		{L".mp3", &SongItemModel::handleSongFile},
		{L".png", &SongItemModel::handleImageFile}
	};

	SongItemModel::SongItemModel(winrt::Windows::Storage::StorageFolder folder) : m_folder{ std::move(folder) }
	{
		for (auto file : m_folder.GetFilesAsync().get())
		{
			auto const fileType = file.FileType();
			if (auto const iter = fileHandlers.find(fileType); iter != fileHandlers.end())
				std::invoke(iter->second, *this, std::move(file));
		}
	}

	winrt::hstring SongItemModel::SongName() const
	{
		return m_songName;
	}

	winrt::hstring SongItemModel::Singer() const
	{
		return m_singer;
	}

	winrt::hstring SongItemModel::Mapper() const
	{
		return m_mapper;
	}

	int SongItemModel::Length() const
	{
		return m_length;
	}

	std::vector<winrt::Windows::Storage::StorageFile> const& SongItemModel::VersionFiles() const
	{
		return m_versionFiles;
	}

	winrt::Windows::Media::Core::MediaSource SongItemModel::Source() const
	{
		return m_songSource;
	}

	winrt::Windows::Foundation::DateTime SongItemModel::DateModified() const
	{
		return std::transform_reduce(
			m_versionFiles.cbegin(),
			m_versionFiles.cend(),
			winrt::Windows::Foundation::DateTime{},
			[](winrt::Windows::Foundation::DateTime time1, winrt::Windows::Foundation::DateTime time2) { return (std::max)(time1, time2); },
			[](winrt::Windows::Storage::StorageFile const& osuFile)
			{
				return osuFile.GetBasicPropertiesAsync().get().DateModified();
			}
		);
	}

	winrt::Windows::Foundation::DateTime SongItemModel::DateCreated() const
	{
		return std::transform_reduce(
			m_versionFiles.cbegin(),
			m_versionFiles.cend(),
			winrt::Windows::Foundation::DateTime{},
			[](winrt::Windows::Foundation::DateTime time1, winrt::Windows::Foundation::DateTime time2) { return (std::max)(time1, time2); },
			[](winrt::Windows::Storage::StorageFile const& osuFile)
			{
				return osuFile.DateCreated();
			}
		);
	}

	std::string SongItemModel::Tags(int versionIndex)
	{
		if (m_versionFiles.empty())
			return "";

		if (versionIndex < 0 || versionIndex >= m_versionFiles.size())
			return "";

		std::string s;
		for (auto const& tag : getMetadata(versionIndex).tags)
			(s += tag) += " ";
		return s;
	}

	int SongItemModel::BitRate() const
	{
		if (!m_songSource)
			return {};

		m_songSource.OpenAsync().get();
		winrt::Windows::Media::Playback::MediaPlaybackItem item{ m_songSource };

		return item.AudioTracks().GetAt(0).GetEncodingProperties().Bitrate();
		//m_songSource.OpenAsync().get();
		//return m_songSource.MediaStreamSource().MusicProperties().Bitrate();
	}

	void SongItemModel::handleOsuFile(winrt::Windows::Storage::StorageFile&& file)
	{
		//Basic informations
		if (m_songName.empty())
			m_songName = winrt::to_hstring(OsuFile::ParseTitleFrom(std::string_view{ winrt::to_string(file.Name()) }));
		if (m_singer.empty())
			m_singer = winrt::to_hstring(OsuFile::ParseCreatorFrom(std::string_view{ winrt::to_string(file.Name()) }));
		if (m_mapper.empty())
			m_mapper = winrt::to_hstring(OsuFile::ParseArtistFrom(std::string_view{ winrt::to_string(file.Name()) }));

		//versions
		m_versionFiles.emplace_back(std::move(file));
		m_metadata.emplace_back();
		//OutputDebugString((m_songName + L" " + m_singer + L" " + m_mapper + L'\n').c_str());
	}

	void SongItemModel::handleSongFile(winrt::Windows::Storage::StorageFile&& file)
	{
		m_songSource = winrt::Windows::Media::Core::MediaSource::CreateFromStorageFile(file);
		m_length = (std::chrono::duration_cast<std::chrono::milliseconds>(file.Properties().GetMusicPropertiesAsync().get().Duration())).count();
	}

	void SongItemModel::handleImageFile(winrt::Windows::Storage::StorageFile&& file)
	{

	}

	Metadata& SongItemModel::getMetadata(int index)
	{
		if (m_metadata[index].has_value())
			return *m_metadata[index];
		assert(m_versionFiles.size() == m_metadata.size());
		Utils::StreambufAdaptor buf{ m_versionFiles[index] };
		m_metadata[index].emplace(Metadata{ std::istream{&buf} });
		return *m_metadata[index];
	}

}