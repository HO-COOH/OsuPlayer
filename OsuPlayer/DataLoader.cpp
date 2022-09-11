#include "pch.h"
#include "DataLoader.h"

using namespace Model;

//std::vector<winrt::Windows::Storage::StorageFolder> DataLoader::LoadOsuFolders()
//{
//    return std::vector<winrt::Windows::Storage::StorageFolder>();
//}
//
//std::vector<SongItemModel> DataLoader::LoadSongs()
//{
//    return std::vector<SongItemModel>();
//}

OverrideLoader& OverrideLoader::GetInstance()
{
    static OverrideLoader s_loader;
    return s_loader;
}

OverrideLoader::OverrideInfo const* OverrideLoader::find(Model::SongItemModel const& song) const
{
    if (auto iter = m_overrideInfo.find(song.getId()); iter == m_overrideInfo.end())
        return nullptr;
    else
        return &(iter->second);
}

void OverrideLoader::setUseSkinHitsound(Model::SongItemModel const& song, bool value)
{
    m_overrideInfo[song.getId()].useSkinHitsound = value;
}

void OverrideLoader::setMod(Model::SongItemModel const& song, winrt::OsuPlayer::ViewModel::PlayMod mod)
{
    m_overrideInfo[song.getId()].mod = mod;
}

std::optional<bool> OverrideLoader::getUseSkinHitsound(Model::SongItemModel const& song)
{
    if (auto ptr = find(song); ptr)
        return ptr->useSkinHitsound;
    return {};
}

std::optional<winrt::OsuPlayer::ViewModel::PlayMod> OverrideLoader::getMod(Model::SongItemModel const& song)
{
    if (auto ptr = find(song); ptr)
        return ptr->mod;
    return {};
}