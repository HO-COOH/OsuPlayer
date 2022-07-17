#pragma once

#include "OsuParser.hpp"
#include <winrt/Windows.Media.h>
#include <winrt/Windows.Media.Core.h>
#include <winrt/Windows.Media.Playback.h>
#include <optional>
#include "PlayMods.h"
#include <fstream>
#include <functional>

namespace Model
{
    class PlayerModel
    {
    public:

        //Return value indicating successful or not
        bool Play();
        void Source(winrt::Windows::Storage::StorageFile file);
        void Source(winrt::Windows::Media::Core::MediaSource source);
        void SetOsuFile(std::ifstream&& file);
        void SetOsuFile(winrt::Windows::Storage::StorageFile file);
        void Pause();


        winrt::Windows::Media::Playback::MediaPlayer m_songPlayer;
        winrt::Windows::Media::Playback::MediaPlayer m_hitSoundPlayer;

    private:
        winrt::Windows::Media::Core::MediaSource m_source{ nullptr };

        std::optional<OsuFile> m_osuFile;

        PlayMods playMod = PlayMods::Normal;
        bool isPlaying = false;
        bool useSkinHitsound{};
    };

}