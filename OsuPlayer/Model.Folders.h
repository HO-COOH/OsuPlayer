#pragma once

#include <winrt/Windows.Storage.h>
#include <vector>
#include <winrt/Windows.Foundation.h>

namespace Model
{
	struct Folders
	{
		[[nodiscard]] bool isFolderUnique(winrt::Windows::Storage::StorageFolder const& folder) const;
		[[nodiscard]] winrt::Windows::Foundation::IAsyncOperation<bool> addOsuFolder(winrt::Windows::Storage::StorageFolder osuFolder);

		//All valid osu song folders
		std::vector<winrt::Windows::Storage::StorageFolder> m_osuFolders;
		std::vector<winrt::Windows::Storage::StorageFolder> m_songFolders{};
		std::vector<winrt::Windows::Storage::StorageFile> m_osuDbs{};
		std::vector<winrt::Windows::Storage::StorageFile> m_collectionDbs{};
		std::vector<winrt::Windows::Storage::StorageFile> m_scoreDbs{};
		std::vector<winrt::Windows::Storage::StorageFolder> m_skinFolders{};

		static Folders& GetInstance();
	private:
		constexpr static inline auto SongFolderName = L"Songs";
		constexpr static inline auto SkinFolderName = L"Skins";
		constexpr static inline auto DbFileName = L"osu!.db";
		constexpr static inline auto CollectionDbFileName = L"collection.db";
		constexpr static inline auto ScoreDbFileName = L"scores.db";
	};

}