/*****************************************************************//**
 * \file   MyMusicModel.h
 * \brief  
 * 
 * \author Peter
 * \date   November 2021
 *********************************************************************/

#pragma once
#include <vector>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Foundation.h>
//#include <winrt/Windows.Foundation.Collections.h>
#include <array>
#include <future>
#include <SongItem.g.h>

struct MyMusicItem
{
	winrt::hstring songName;
	winrt::hstring singer;
	winrt::hstring mapper;
	std::vector<winrt::hstring> versions;
};

class MyMusicModel
{
public:
	MyMusicModel() = default;
	MyMusicModel(winrt::Windows::Storage::StorageFolder folder);
	MyMusicModel(winrt::hstring const& folderPath);

	winrt::Windows::Foundation::IAsyncAction setPath(winrt::hstring const& folderPath);

	template<size_t Count>
	std::future<std::array<MyMusicItem, Count>> readSomeAsync();

	winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Foundation::Collections::IVector<winrt::OsuPlayer::SongItem>> readSomeAsync(size_t count);
private:
	winrt::Windows::Storage::StorageFolder m_folder{ nullptr };
	size_t currentIndex{};
};

template<size_t Count>
inline std::future<std::array<MyMusicItem, Count>> MyMusicModel::readSomeAsync()
{
	return std::array<MyMusicItem, Count>();
}
