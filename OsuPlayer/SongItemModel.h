#pragma once

#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Foundation.h>
#include <vector>
#include <unordered_map>
#include <winrt/Windows.Media.Core.h>

class SongItemModel
{
public:
	SongItemModel() = default;
	explicit SongItemModel(winrt::Windows::Storage::StorageFolder folder);
	winrt::hstring SongName() const;
	winrt::hstring Singer() const;
	winrt::hstring Mapper() const;
	int Length() const;
	std::vector<winrt::Windows::Storage::StorageFile> const& VersionFiles() const;
	winrt::Windows::Media::Core::MediaSource Source() const;
	winrt::Windows::Foundation::DateTime DateModified() const;
	winrt::Windows::Foundation::DateTime DateCreated() const;
	std::string Tags(int versionIndex) const;
	int BitRate() const;
private:
	void handleOsuFile(winrt::Windows::Storage::StorageFile&& file);
	void handleSongFile(winrt::Windows::Storage::StorageFile&& file);
	void handleImageFile(winrt::Windows::Storage::StorageFile&& file);
public:
	using FileHandlerFunction = decltype(&handleOsuFile);
	using FileExtension = winrt::hstring;
private:
	/**
	 * @brief Stores the file handler function according to different file extensions
	 * @details For example, audio files can either be .mp3 or .wav, and they should be registered with handleSongFile() in this unordered_map
	 */
	static std::unordered_map<FileExtension, FileHandlerFunction> fileHandlers;

	int m_length{};	//in milliseconds
	winrt::hstring m_songName;
	winrt::hstring m_singer;
	winrt::hstring m_mapper;
	winrt::Windows::Media::Core::MediaSource m_songSource{nullptr};
	winrt::Windows::Storage::StorageFolder m_folder{nullptr};
	std::vector<winrt::Windows::Storage::StorageFile> m_versionFiles;
};

