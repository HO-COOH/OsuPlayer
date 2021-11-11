#include "pch.h"
#include "MyMusicModel.h"
#include "OsuParser.hpp"
#include <filesystem>
#include <winrt/Windows.Storage.Search.h>

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
		[this, count]()
		{
			std::wstring_view const pathWstr = m_folder.Path();
			if (pathWstr.substr(pathWstr.rfind('\\')) == L"songs")
			{
				/*the path is inside song folder*/
				std::vector<MyMusicItem> vec;
				//for(auto item :std::filesystem::directory_iterator{ std::filesystem::path{ pathWstr } })
				//{

				//}
				auto folders = m_folder.GetFoldersAsync(winrt::Windows::Storage::Search::CommonFolderQuery::DefaultQuery, currentIndex, count).get();
				for(auto folder: folders)
				{
					
					auto files = folder.GetFilesAsync().get();
					MyMusicItem item;
					for(auto file : files)
					{
						if (file.FileType() == L"osu")
						{
							if (item.songName.empty())
								item.songName = winrt::to_hstring(OsuFile::ParseTitleFrom(std::string_view{ winrt::to_string(file.Name()) }));
							if (item.mapper.empty())
								item.mapper = winrt::to_hstring(OsuFile::ParseCreatorFrom(std::string_view{ winrt::to_string(file.Name()) }));
							if (item.singer.empty())
								item.singer = winrt::to_hstring(OsuFile::ParseArtistFrom(std::string_view{ winrt::to_string(file.Name()) }));
							item.versions.emplace_back(winrt::to_hstring(OsuFile::ParseVersionFrom(std::string_view{ winrt::to_string(file.Name()) })));
						}
					}
					vec.emplace_back(std::move(item));
				}
				return vec;
			}
			else
			{
				return std::vector<MyMusicItem>{};
			}
		}
	);
}

