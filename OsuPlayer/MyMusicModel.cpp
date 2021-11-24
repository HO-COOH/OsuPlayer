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

winrt::Windows::Foundation::IAsyncAction MyMusicModel::setPath(winrt::hstring const& folderPath)
{
	m_folder = co_await winrt::Windows::Storage::StorageFolder::GetFolderFromPathAsync(folderPath);
}

winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Foundation::Collections::IVector<winrt::OsuPlayer::SongItem>> MyMusicModel::readSomeAsync(size_t count)
{
	std::wstring const path{ m_folder.Path() };
	if (path.substr(path.rfind('\\')) == L"\\Songs")
	{
		/*the path is inside song folder*/
		std::vector<winrt::OsuPlayer::SongItem> vec;
		//for(auto item :std::filesystem::directory_iterator{ std::filesystem::path{ pathWstr } })
		//{

		//}
		auto folders = co_await m_folder.GetFoldersAsync(winrt::Windows::Storage::Search::CommonFolderQuery::DefaultQuery, currentIndex, count);
		for(auto folder: folders)
		{
			
			auto files = co_await folder.GetFilesAsync();
			winrt::OsuPlayer::SongItem item;
			for(auto file : files)
			{
				auto const fileType = file.FileType();
				if (fileType == L".osu")
				{
					if (item.SongName().empty())
						item.SongName(winrt::to_hstring(OsuFile::ParseTitleFrom(std::string_view{ winrt::to_string(file.Name()) })));
					if (item.Mapper().empty())
						item.Mapper(winrt::to_hstring(OsuFile::ParseCreatorFrom(std::string_view{ winrt::to_string(file.Name()) })));
					if (item.Singer().empty())
						item.Singer(winrt::to_hstring(OsuFile::ParseArtistFrom(std::string_view{winrt::to_string(file.Name())})));
					//item.Vers.emplace_back(winrt::to_hstring(OsuFile::ParseVersionFrom(std::string_view{ winrt::to_string(file.Name()) })));
				}
				else if (fileType == L".mp3")
				{
					item.SongFile(file);
				}
			}
			vec.emplace_back(std::move(item));
		}
		co_return winrt::single_threaded_vector(std::move(vec));
	}
	else
	{
		co_return winrt::single_threaded_vector<winrt::OsuPlayer::SongItem>();
	}
}

