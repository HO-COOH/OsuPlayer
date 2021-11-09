#include "pch.h"
#include "MyMusicModel.h"
#include "OsuParser.hpp"
#include <filesystem>

MyMusicModel::MyMusicModel(winrt::Windows::Storage::StorageFolder folder)
	:m_folder{std::move(folder)}
{
}

MyMusicModel::MyMusicModel(winrt::hstring const& folderPath)
	: MyMusicModel(winrt::Windows::Storage::StorageFolder::GetFolderFromPathAsync(folderPath).get())
{
}

std::future<std::vector<MyMusicItem>> MyMusicModel::readSomeAsync(size_t count)
{
	return std::async(
		std::launch::async,
		[this]()
		{
			std::wstring_view const pathWstr = m_folder.Path();
			if (pathWstr.substr(pathWstr.rfind('\\')) == L"songs")
			{
				/*the path is inside song folder*/
				m_folder.GetFolderAsync(winrt::Windows::Storage::StorageFolder::Comm)
			}
			else
			{
				/*the path is inside osu folder*/
			}
		}
	);
}

