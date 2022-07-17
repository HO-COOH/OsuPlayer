#include "pch.h"
#include "Model.Player.h"
#include <winrt/Windows.Storage.h>
#include "Utils.Log.hpp"

namespace Model
{
    bool PlayerModel::Play()
    {
        m_songPlayer.Play();
        return static_cast<bool>(m_songPlayer.Source());
    }

    void PlayerModel::Source(winrt::Windows::Storage::StorageFile file)
    {
        Source(winrt::Windows::Media::Core::MediaSource::CreateFromStorageFile(file));
    }

    void PlayerModel::Source(winrt::Windows::Media::Core::MediaSource source)
    {
        m_songPlayer.Pause();
        m_songPlayer.Source(source);
        m_songPlayer.Play();
    }

    void PlayerModel::SetOsuFile(std::ifstream&& file)
    {
        m_osuFile.emplace(std::move(file));
    }

    void PlayerModel::SetOsuFile(winrt::Windows::Storage::StorageFile file)
    {
        SetOsuFile(std::ifstream{ file.Path().data() });
    }

    void PlayerModel::Pause()
    {
        m_songPlayer.Pause();
    }
}