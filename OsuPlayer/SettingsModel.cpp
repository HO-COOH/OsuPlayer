#include "pch.h"
#include "SettingsModel.h"
#include <winrt/Windows.Storage.Pickers.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <algorithm>
#include "MyMusicModel.h"

InvalidOsuFolderException::InvalidOsuFolderException(winrt::Windows::Storage::StorageFolder folder)
    : m_folder{ folder }
{
}

winrt::Windows::Storage::StorageFolder const& InvalidOsuFolderException::Folder() const
{
    return m_folder;
}

char const* InvalidOsuFolderException::what() const
{
    return "Incorrect Osu folder";
}

template<typename T>
inline void PushIfNotNull(std::vector<T>& vec, T object)
{
    if (object)
        vec.push_back(object);
}

winrt::Windows::Foundation::IAsyncAction SettingsModel::doPickOsuFolder()
{
    winrt::Windows::Storage::Pickers::FolderPicker picker;
    picker.ViewMode(winrt::Windows::Storage::Pickers::PickerViewMode::List);

    auto folder = co_await picker.PickSingleFolderAsync();
    auto newSongFolder = co_await folder.GetFolderAsync(L"Songs");
    if (isFolderUnique(newSongFolder))
    {
        PushIfNotNull(m_osuFolders, newSongFolder);
        PushIfNotNull(m_osuDbs, co_await folder.GetFileAsync(L"osu!.db"));
        PushIfNotNull(m_collectionDbs, co_await folder.GetFileAsync(L"collection.db"));
        PushIfNotNull(m_scoreDbs, co_await folder.GetFileAsync(L"scores.db"));
        PushIfNotNull(m_skinFolders, co_await folder.GetFolderAsync(L"Skins"));
    }
    else
        throw InvalidOsuFolderException{folder};
}

std::vector<winrt::Windows::Storage::StorageFolder> const& SettingsModel::OsuFolders()
{
    return m_osuFolders;
}

PlayMods SettingsModel::Mod() const
{
    return m_mod;
}

void SettingsModel::Mod(PlayMods mod)
{
    m_mod = mod;
}



bool SettingsModel::isFolderUnique(winrt::Windows::Storage::StorageFolder const& folder) const
{
    return std::find_if(
        m_osuFolders.cbegin(),
        m_osuFolders.cend(),
        [&folder](winrt::Windows::Storage::StorageFolder const& m_osufolder)
        {
            return folder.Path() == m_osufolder.Path();
        }
    ) == m_osuFolders.cend();
}