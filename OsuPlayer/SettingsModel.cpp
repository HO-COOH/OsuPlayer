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

winrt::Windows::Foundation::IAsyncAction SettingsModel::doPickOsuFolder()
{
    winrt::Windows::Storage::Pickers::FolderPicker picker;
    picker.ViewMode(winrt::Windows::Storage::Pickers::PickerViewMode::List);

    if (auto folder = co_await picker.PickSingleFolderAsync(); co_await IsFolderValid(folder) && isFolderUnique(folder))
    {
        m_osuFolders.push_back(co_await folder.GetFolderAsync(L"Songs"));
    }
    else
        throw InvalidOsuFolderException{ folder };
}

std::vector<winrt::Windows::Storage::StorageFolder> const& SettingsModel::OsuFolders() const
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

std::future<bool> SettingsModel::IsFolderValid(winrt::Windows::Storage::StorageFolder const& folder)
{
    return std::async(
        std::launch::async,
        [folder]
        {
            return folder && folder.GetFolderAsync(L"Skins").get() && folder.GetFolderAsync(L"Songs").get();
        }
        );
}


