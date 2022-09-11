#include "pch.h"
#include "ViewModel.Player.h"
#if __has_include("ViewModel.PlayerViewModel.g.cpp")
#include "ViewModel.PlayerViewModel.g.cpp"
#endif

#include "Model.MyMusic.h"
#include "Utils.h"
#include "Model.Recent.h"
#include "OsuParser.hpp"
#include "ViewModelLocator.h"
#include <chrono>
#include "DataLoader.h"

using namespace Model;

namespace winrt::OsuPlayer::ViewModel::implementation
{
	static inline double NormalizeVolume(int volume)
	{
		return volume / 100.0;
	}

	static inline int UnnormalizeVolume(double volume)
	{
		return static_cast<int>(round(volume * 100.0));
	}

	PlayerViewModel::PlayerViewModel()
	{
		m_songPlayer.PlaybackSession().PositionChanged(
			[this](winrt::Windows::Media::Playback::MediaPlaybackSession session, 
				winrt::Windows::Foundation::IInspectable) -> winrt::Windows::Foundation::IAsyncAction
			{
				m_progress = session.Position().count() / 10'000ll;
				co_await winrt::resume_foreground(winrt::Windows::ApplicationModel::Core::CoreApplication::MainView().CoreWindow().Dispatcher());
				raisePropertyChange(L"Progress");
			}
		);

		auto settings = winrt::Windows::Storage::ApplicationData::Current().LocalSettings().Values();
		m_mixedVolume = winrt::unbox_value_or<int>(settings.TryLookup(L"GlobalVolume"), 100);
		SongVolume(winrt::unbox_value_or<int>(settings.TryLookup(L"SongVolume"), 100), false);
		HitsoundVolume(winrt::unbox_value_or<int>(settings.TryLookup(L"HitsoundVolume"), 100), false);
		
		
		raisePropertyChange(L"Volume");
		raisePropertyChange(L"SongVolume");
		raisePropertyChange(L"HitsoundVolume");
	}

	winrt::Windows::Foundation::IAsyncAction PlayerViewModel::Play(ViewModel::SongItemViewModel item)
	{
		if (m_currentItemToPlay)
			m_currentItemToPlay.IsPlaying(false);
		auto& songItemModel = *reinterpret_cast<SongItemModel*>(winrt::unbox_value<size_t>(item.ModelPointer()));
		co_await songItemModel.fillDataAsync();
		co_await songItemModel.Source().OpenAsync();
		co_await m_hitSoundPlayer.init();
		m_hitSoundPlayer.setFolder(item.Folder());
		m_songPlayer.Pause();
		m_songPlayer.Source(nullptr);
		item.IsPlaying(true);
		m_currentItemToPlay = item;
		winrt::Windows::Media::Playback::MediaPlaybackItem playbackItem{songItemModel.Source()};
		co_await updateForSMTC(playbackItem);

		//Add timed metadata
		auto const offset = ViewModelLocator::Current().SettingsViewModel().Offset();
		winrt::Windows::Media::Core::TimedMetadataTrack hitObjectsMeta{ L"0", L"en-US", winrt::Windows::Media::Core::TimedMetadataKind::Data };
		hitObjectsMeta.Label(L"Custom data track");
		hitObjectsMeta.CueEntered([this, &songItemModel](
			winrt::Windows::Media::Core::TimedMetadataTrack const&, 
			winrt::Windows::Media::Core::MediaCueEventArgs args)
		{
			auto hitObjectPtr = *reinterpret_cast<int*>(
				args
				.Cue()
				.as<winrt::Windows::Media::Core::DataCue>()
				.Data()
				.data());

			auto const& beatmap = songItemModel.m_beatmaps[m_currentItemToPlay.SelectedVersionIndex()].originalFile;
			auto const& hitObject = *beatmap->hitObjects[hitObjectPtr];
			while (m_timingPointIter < beatmap->timingPoints.cend() - 1)
			{
				if (hitObject.time >= (m_timingPointIter + 1)->time)
					++m_timingPointIter;
				else
					break;
			}
			m_hitSoundPlayer.playHitsound(hitObject, std::addressof(*m_timingPointIter));
		});

		
		//Maybe try using index instead of using pointers to hit object
		auto const& beatmapFile = songItemModel.m_beatmaps[item.SelectedVersionIndex()].originalFile;
		m_timingPointIter = beatmapFile->timingPoints.begin();
		int i = 0;
		for (auto const& hitObject : beatmapFile->hitObjects)
		{
			winrt::Windows::Media::Core::DataCue cue;
			cue.StartTime(std::chrono::milliseconds{ hitObject->time - offset });
			cue.Duration(std::chrono::milliseconds{ 100 });
			winrt::Windows::Storage::Streams::Buffer b{ sizeof(int) };
			*(int*)(b.data()) = i++;
			cue.Data(b);
			hitObjectsMeta.AddCue(cue);
		}

		playbackItem.Source().ExternalTimedMetadataTracks().Append(hitObjectsMeta);
		playbackItem.TimedMetadataTracks().SetPresentationMode(
			0, 
			winrt::Windows::Media::Playback::TimedMetadataTrackPresentationMode::ApplicationPresented);

		m_songPlayer.Source(playbackItem);
		m_songPlayer.Play();
		
		//update ui
		raisePropertyChange(L"SongLength");
		raisePropertyChange(L"HasItems");

		//update recent data
		static auto recentInstance = ViewModelLocator::Current().RecentViewModel();
		recentInstance.addSongRecord(item);
		//recentInstance.updateJumpList();
	}

	void PlayerViewModel::PlayCurrent()
	{
	}

	void PlayerViewModel::PlayPrevious()
	{
	}

	void PlayerViewModel::PlayNext()
	{
	}

	winrt::Windows::Foundation::IAsyncAction PlayerViewModel::PlayList(winrt::Windows::Media::Playback::MediaPlaybackList list)
	{
		co_return;
	}

	PlayMod PlayerViewModel::Mod()
	{
		if (m_currentItemToPlay)
		{
			if (auto value = OverrideLoader::GetInstance().getMod(*reinterpret_cast<SongItemModel*>(winrt::unbox_value<size_t>(m_currentItemToPlay.ModelPointer()))); 
				value.has_value())
				return *value;
		}
		return static_cast<PlayMod>(ViewModelLocator::Current().SettingsViewModel().DefaultMod());
	}

	void PlayerViewModel::Mod(PlayMod mod)
	{
		if (m_currentItemToPlay)
		{
			OverrideLoader::GetInstance().setMod(*reinterpret_cast<SongItemModel*>(winrt::unbox_value<size_t>(m_currentItemToPlay.ModelPointer())), mod);
			raisePropertyChange(L"ModString");
			raisePropertyChange(L"IsModEnabled");
		}
	}

	bool PlayerViewModel::IsModEnabled()
	{
		return Mod() != PlayMod::Normal;
	}

	void PlayerViewModel::IsModEnabled(bool)
	{
		//this property should not be set
		assert(false);
	}

	bool PlayerViewModel::UseSkinHitsound()
	{
		if (auto value = OverrideLoader::GetInstance().getUseSkinHitsound(*reinterpret_cast<SongItemModel*>(winrt::unbox_value<size_t>(m_currentItemToPlay.ModelPointer())));
			value.has_value())
			return *value;
		return {};
	}

	void PlayerViewModel::UseSkinHitsound(bool useSkinHitsound)
	{
		OverrideLoader::GetInstance().setUseSkinHitsound(*reinterpret_cast<SongItemModel*>(winrt::unbox_value<size_t>(m_currentItemToPlay.ModelPointer())));
	}

	int PlayerViewModel::Progress()
	{
		return m_progress;
	}

	int PlayerViewModel::SongLength()
	{
		if (m_currentItemToPlay)
			return m_currentItemToPlay.Length();
		else
			return 0;
	}

	int PlayerViewModel::Volume()
	{
		return m_mixedVolume;
	}

	void PlayerViewModel::Volume(int volume)
	{
		static auto Settings = winrt::Windows::Storage::ApplicationData::Current()
			.LocalSettings()
			.Values();
		auto songVolumeBefore = SongVolume();
		auto hitsoundVolumeBefore = HitsoundVolume();
		m_mixedVolume = volume;
		SongVolume(songVolumeBefore, false);
		HitsoundVolume(hitsoundVolumeBefore, false);
		raisePropertyChange(L"SongVolume");
		raisePropertyChange(L"HitsoundVolume");
		raisePropertyChange(L"Volume");
		Settings.Insert(L"GlobalVolume", winrt::box_value(volume));
	}

	int PlayerViewModel::SongVolume()
	{
		return UnnormalizeVolume(m_songPlayer.Volume() / NormalizeVolume(m_mixedVolume));
	}

	void PlayerViewModel::SongVolume(int songVolume, bool writeToSetting)
	{
		static auto Settings = winrt::Windows::Storage::ApplicationData::Current()
			.LocalSettings()
			.Values();
		m_songPlayer.Volume(
			NormalizeVolume(songVolume) * NormalizeVolume(m_mixedVolume)
		);

		if(writeToSetting)
			Settings.Insert(L"SongVolume", winrt::box_value(songVolume));
	}

	int PlayerViewModel::HitsoundVolume()
	{
		return UnnormalizeVolume(m_hitSoundPlayer.NormalizedVolume() / NormalizeVolume(m_mixedVolume));
	}

	void PlayerViewModel::HitsoundVolume(int hitsoundVolume, bool writeToSetting)
	{
		static auto Settings = winrt::Windows::Storage::ApplicationData::Current()
			.LocalSettings()
			.Values();
		m_hitSoundPlayer.NormalizedVolume(
			NormalizeVolume(hitsoundVolume) * NormalizeVolume(m_mixedVolume)
		);

		if(writeToSetting)
			Settings.Insert(L"HitsoundVolume", winrt::box_value(hitsoundVolume));
	}

	void PlayerViewModel::Mute()
	{
		if (m_muteInfo.isMute)
		{
			m_muteInfo.songVolumeBefore = m_songPlayer.Volume();
			m_muteInfo.hitsoundVolumeBefore = m_hitSoundPlayer.NormalizedVolume();
			m_muteInfo.globalVolumeBefore = Volume();
			Volume(0);
			m_muteInfo.isMute = true;
		}
		else
		{
			m_songPlayer.Volume(m_muteInfo.songVolumeBefore);
			m_hitSoundPlayer.NormalizedVolume(m_muteInfo.hitsoundVolumeBefore);
			Volume(UnnormalizeVolume(m_muteInfo.globalVolumeBefore));
			m_muteInfo.isMute = false;
		}
	}

	void PlayerViewModel::Save()
	{
		static auto settings = winrt::Windows::Storage::ApplicationData::Current().LocalSettings().Values();
		settings.Insert(L"GlobalVolume", winrt::box_value(Volume()));
		settings.Insert(L"SongVolume", winrt::box_value(SongVolume()));
		settings.Insert(L"HitsoundVolume", winrt::box_value(HitsoundVolume()));
	}

	winrt::Windows::UI::Xaml::Media::ImageSource PlayerViewModel::ImageSource()
	{
		return winrt::Windows::UI::Xaml::Media::ImageSource{ nullptr };
	}
	
	PlayerViewModel::~PlayerViewModel()
	{
		Save();
	}

	winrt::Windows::Foundation::IAsyncAction PlayerViewModel::updateForSMTC(winrt::Windows::Media::Playback::MediaPlaybackItem item)
	{
		auto properties = item.GetDisplayProperties();
		auto musicProperty = properties.MusicProperties();
		musicProperty.Title(m_currentItemToPlay.SongName());
		musicProperty.Artist(m_currentItemToPlay.Singer());

		auto songImageFile = co_await m_currentItemToPlay.SongImageFile();
		if (songImageFile != nullptr)
		{
			//co_await stream.OpenReadAsync();
			properties.Thumbnail(winrt::Windows::Storage::Streams::RandomAccessStreamReference::CreateFromFile(songImageFile));
		}
		properties.Type(winrt::Windows::Media::MediaPlaybackType::Music);
		item.ApplyDisplayProperties(properties);
		co_return;
	}
	void PlayerViewModel::updateForSMTC()
	{

	}
}
