#include "pch.h"
#include "PlayerModel.h"
#include <winrt/Windows.Media.h>
#include <winrt/Windows.Media.Playback.h>
#include <winrt/Windows.Storage.h>

void PlayerModel::Play()
{
    m_songPlayer.Play();
}

void PlayerModel::Source(winrt::Windows::Storage::StorageFile file)
{
    m_songPlayer.Source(winrt::Windows::Media::Core::MediaSource::CreateFromStorageFile(file));
    m_songPlayer.Play();
}

void PlayerModel::SetOsuFile(std::ifstream&& file)
{
    m_osuFile.emplace(std::move(file));
}

void PlayerModel::SetOsuFile(winrt::Windows::Storage::StorageFile file)
{
    SetOsuFile(std::ifstream{ file.Path().data()});
}

void PlayerModel::Pause()
{
    m_songPlayer.Pause();
}
