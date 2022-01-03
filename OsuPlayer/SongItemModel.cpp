#define NOMINMAX
#include "pch.h"
#include "SongItemModel.h"
#include "OsuParser.hpp"
#include <chrono>
#include <winrt/Windows.Storage.FileProperties.h>
#include <algorithm>

std::unordered_map<winrt::hstring, SongItemModel::FileHandlerFunction> SongItemModel::fileHandlers
{
	{L".osu", &SongItemModel::handleOsuFile},
	{L".mp3", &SongItemModel::handleSongFile},
	{L".png", &SongItemModel::handleImageFile}
};

SongItemModel::SongItemModel(winrt::Windows::Storage::StorageFolder folder) : m_folder{std::move(folder)}
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

#include "FileStreamAdaptor.hpp"
std::string SongItemModel::Tags(int versionIndex) const
{
	if (m_versionFiles.empty())
		return "";

	if (versionIndex < 0 || versionIndex >= m_versionFiles.size())
		return "";

	//auto file = m_versionFiles[versionIndex].CopyAsync(
	//	winrt::Windows::Storage::ApplicationData::Current().LocalFolder(), 
	//	L"temp.txt", 
	//	winrt::Windows::Storage::NameCollisionOption::ReplaceExisting
	//).get();
	//You got file access except when it's not, so you can't directly construct std::ifstream from StorageFile::Path()
	//Metadata meta{std::ifstream{std::wstring{m_versionFiles[versionIndex].Path()}}};
	StreambufAdaptor buf{ m_versionFiles[versionIndex] };

	Metadata meta{ std::istream{&buf} };
	std::string s;
	for (auto const& tag : meta.tags)
		(s += tag) += " ";
	return s;
}

#include <winrt/Windows.Media.Playback.h>
#include <winrt/Windows.Media.MediaProperties.h>
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

#include "Log.h"
void SongItemModel::handleOsuFile(winrt::Windows::Storage::StorageFile && file)
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
