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
//#include <winrt/Windows.Foundation.Collections.h>
#include <array>
#include <future>

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
	MyMusicModel(winrt::Windows::Storage::StorageFolder folder);
	MyMusicModel(winrt::hstring const& folderPath);

	template<size_t Count>
	std::future<std::array<MyMusicItem, Count>> readSomeAsync();

	std::future<std::vector<MyMusicItem>> readSomeAsync(size_t count);
private:
	winrt::Windows::Storage::StorageFolder m_folder;
};

template<size_t Count>
inline std::future<std::array<MyMusicItem, Count>> MyMusicModel::readSomeAsync()
{
	return std::array<MyMusicItem, Count>();
}
