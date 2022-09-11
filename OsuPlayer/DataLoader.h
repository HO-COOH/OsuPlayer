#pragma once

#include <winrt/Windows.Storage.h>
#include <unordered_map>
#include "Model.SongItem.h"
#include "ViewModel.PlayerViewModel.g.h"
//class DataLoader
//{
//public:
//	static std::vector<winrt::Windows::Storage::StorageFolder> LoadOsuFolders();
//	static std::vector<Model::SongItemModel> LoadSongs();
//private:
//
//};


/**
 * @brief This class loads override info data from a db file
 * @details Overridable data includes: 
*/
class OverrideLoader
{
public:

	class OverrideInfo
	{
		//We use std::optional here, because when a new override item is set in the std::unordered_map,
		//the values are zero-initialized, so we have no way to distinguish whether it is actually override 
		//or just because of initialized to be so
		std::optional<bool> useSkinHitsound{};
		std::optional<winrt::OsuPlayer::ViewModel::PlayMod> mod{};
	public:
		friend class OverrideLoader;
	};

	static OverrideLoader& GetInstance();
	
	void setUseSkinHitsound(Model::SongItemModel const& song, bool value = true);
	std::optional<bool> getUseSkinHitsound(Model::SongItemModel const& song);

	void setMod(Model::SongItemModel const& song, winrt::OsuPlayer::ViewModel::PlayMod mod);
	std::optional<winrt::OsuPlayer::ViewModel::PlayMod> getMod(Model::SongItemModel const& song);

private:
	OverrideInfo const* find(Model::SongItemModel const& song) const;
	using SongIdType = std::remove_cvref_t<decltype(Model::SongItemModel{}.getId())>;
	std::unordered_map<SongIdType, OverrideInfo> m_overrideInfo;
};