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
	/**
	 * \brief Opens a file picker dialog to choose an osu folder, stores the folder in `m_osuFolders`
	 */
	winrt::Windows::Foundation::IAsyncAction doPickOsuFolder();

	/**
	 * \brief Get osu folders
	 */
	std::vector<winrt::Windows::Storage::StorageFolder> const& OsuFolders() const;

	[[nodiscard]]PlayMods Mod() const;

	void Mod(PlayMods mod);
private:
	
	//All valid osu folders
	inline static std::vector<winrt::Windows::Storage::StorageFolder> m_osuFolders{};


	PlayMods m_mod;

	[[nodiscard]]bool isFolderUnique(winrt::Windows::Storage::StorageFolder const& folder) const;

	/**
	 * \brief Helper function to determines whether a path is a valid osu folder
	 * 
	 * \retval true  If the path is a valid osu folder (i.e. has "Songs" and "Skins" subfolder)
	 * \retval false If the path is NOT a valid osu folder (i.e. does NOT have "Songs" and "Skins" subfolder)
	 */
	static std::future<bool> IsFolderValid(winrt::Windows::Storage::StorageFolder const& folder);

	friend class MyMusicModel;
};

