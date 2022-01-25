#pragma once

#include <exception>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/base.h>
#include <vector>
#include "PlayMods.h"
#include <future>

/**
 * Represents an exception that will be thrown when the folder picked by user
 * is an incorrect folder.
 */
class InvalidOsuFolderException : public std::exception
{
public:
	/**
	 * \brief Constructor of the IncorrectOsuFolderException
	 * 
	 * \param folder The invalid osu folder
	 */
	InvalidOsuFolderException(winrt::Windows::Storage::StorageFolder folder);

	/**
	 * \brief Get the invalid osu folder that causes this exception to occur
	 * 
	 * \return The incorrect osu folder that causes this exception to occur
	 */
	winrt::Windows::Storage::StorageFolder const& Folder() const;

	char const* what() const override;

private:
	winrt::Windows::Storage::StorageFolder m_folder;
};

class SettingsModel
{ 
public:
	enum class LocalDataType
	{
		SongFolder,
		OsuDB,
		CollectionDB,
		ScoreDB,
		SkinFolder
	};
	/**
	 * \brief Opens a file picker dialog to choose an osu folder, stores the folder in `m_osuFolders`
	 */
	winrt::Windows::Foundation::IAsyncAction doPickOsuFolder();

	/**
	 * \brief Get osu folders
	 */
	static std::vector<winrt::Windows::Storage::StorageFolder> const& OsuFolders();

	template<LocalDataType localDataType>
	static auto GetLocalData()
	{
		if constexpr (localDataType == LocalDataType::SongFolder) return m_osuFolders;
		else if constexpr (localDataType == LocalDataType::OsuDB) return m_osuDbs;
		else if constexpr (localDataType == LocalDataType::CollectionDB) return m_collectionDbs;
		else if constexpr (localDataType == LocalDataType::ScoreDB) return m_scoreDbs;
		else if constexpr (localDataType == LocalDataType::SkinFolder) return m_skinFolders;
	}

	[[nodiscard]]PlayMods Mod() const;

	void Mod(PlayMods mod);
private:
	
	//All valid osu song folders
	inline static std::vector<winrt::Windows::Storage::StorageFolder> m_osuFolders{};
	inline static std::vector<winrt::Windows::Storage::StorageFile> m_osuDbs{};
	inline static std::vector<winrt::Windows::Storage::StorageFile> m_collectionDbs{};
	inline static std::vector<winrt::Windows::Storage::StorageFile> m_scoreDbs{};
	inline static std::vector<winrt::Windows::Storage::StorageFolder> m_skinFolders{};


	PlayMods m_mod;

	[[nodiscard]]bool isFolderUnique(winrt::Windows::Storage::StorageFolder const& folder) const;

	friend class MyMusicModel;
};

