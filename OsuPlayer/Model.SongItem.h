#pragma once

#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Foundation.h>
#include <vector>
#include <unordered_map>
#include <winrt/Windows.Media.Core.h>
#include "Utils.FileStreamAdaptor.hpp"
#include "OsuDBParser.hpp"
#include "OsuParser.hpp"
#include <optional>
#include <ranges>
namespace Model
{
	struct BeatmapInfo
	{
		Db::Beatmap const* beatmapPtr;
		winrt::Windows::Foundation::DateTime dateModified;
		winrt::Windows::Foundation::DateTime dateCreated;
		std::optional<OsuFile> originalFile;

		void reset() { originalFile.reset(); }
	};

	class SongItemModel
	{
	public:
		SongItemModel() = default;
		winrt::hstring SongName() const;
		winrt::hstring Singer() const;
		winrt::hstring Mapper() const;
		int Length() const;
		auto Difficulties() const 
		{ 
			return m_beatmaps  | std::views::transform(
				[](BeatmapInfo const& beatmapInfo)
				{ 
					return winrt::to_hstring(beatmapInfo.beatmapPtr->difficulty); 
				});
		}
		winrt::Windows::Media::Core::MediaSource Source() const;
		winrt::Windows::Foundation::DateTime DateModified() const;
		winrt::Windows::Foundation::DateTime DateCreated() const;
		std::string Tags(int versionIndex);
		int BitRate() const;
		bool isDataFilled() const;
		std::vector<BeatmapInfo> m_beatmaps;

		winrt::Windows::Foundation::IAsyncAction fillDataAsync();
	private:
		void handleImageFile(winrt::Windows::Storage::StorageFile&& file);

		int m_length{};	//in milliseconds
		int m_bitrate{};
		
		winrt::Windows::Media::Core::MediaSource m_songSource{ nullptr };
		winrt::Windows::Storage::StorageFolder m_folder{ nullptr };

		Metadata& getMetadata(int index);

		////Return the folder of this beatmap set
		//winrt::Windows::Foundation::IAsyncAction getFolder();

		//Return the file of a beatmap
		winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Storage::StorageFile> getFileOf(BeatmapInfo const& info);

	};

}