#pragma once

#include "OsuParser.hpp"
#include <winrt/Windows.Media.Core.h>
#include <optional>
#include "PlayMods.h"
#include <fstream>

class PlayerModel
{
public:


    void Play();
    void Source(winrt::Windows::Storage::StorageFile file);
    void SetOsuFile(std::ifstream&& file);
    void SetOsuFile(winrt::Windows::Storage::StorageFile file);
    void Pause();
    

private:

    winrt::Windows::Media::Playback::MediaPlayer m_songPlayer;
    winrt::Windows::Media::Playback::MediaPlayer m_hitSoundPlayer;
    winrt::Windows::Media::Core::MediaSource m_source{ nullptr };
    std::optional<OsuFile> m_osuFile;

    PlayMods playMod = PlayMods::Normal;
    bool isPlaying = false;
    bool useSkinHitsound{};
};

