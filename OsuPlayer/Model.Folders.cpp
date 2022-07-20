#include "pch.h"
#include "Model.Folders.h"
#include <winrt/Windows.Storage.h>
#include <algorithm>
#include "Utils.h"
#include "Model.Folders.h"

namespace Model
{


    bool Folders::isFolderUnique(winrt::Windows::Storage::StorageFolder const& folder) const
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

    Folders& Folders::GetInstance()
    {
        static Folders folders;
        return folders;
    }

    winrt::Windows::Foundation::IAsyncOperation<bool> Folders::addOsuFolder(winrt::Windows::Storage::StorageFolder osuFolder)
    {
        auto songFolder = co_await osuFolder.GetFolderAsync(SongFolderName);
        if (!songFolder)
            co_return false;
        m_osuFolders.push_back(osuFolder);
        m_songFolders.push_back(songFolder);
        Utils::PushIfNotNull(m_osuDbs, co_await osuFolder.GetFileAsync(DbFileName));
        Utils::PushIfNotNull(m_collectionDbs, co_await osuFolder.GetFileAsync(CollectionDbFileName));
        Utils::PushIfNotNull(m_scoreDbs, co_await osuFolder.GetFileAsync(ScoreDbFileName));
        Utils::PushIfNotNull(m_skinFolders, co_await osuFolder.GetFolderAsync(SkinFolderName));
        co_return true;
    }
}