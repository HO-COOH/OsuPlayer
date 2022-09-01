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

using namespace Model;

namespace winrt::OsuPlayer::ViewModel::implementation
{
	PlayerViewModel::PlayerViewModel()
	{
		m_songPlayer.PlaybackSession().PositionChanged(
			[this](winrt::Windows::Media::Playback::MediaPlaybackSession session, winrt::Windows::Foundation::IInspectable _)->winrt::Windows::Foundation::IAsyncAction
			{
				m_progress = session.Position().count() / 10'000ll;
				co_await winrt::resume_foreground(winrt::Windows::ApplicationModel::Core::CoreApplication::MainView().CoreWindow().Dispatcher());
				raisePropertyChange(L"Progress");
			}
		);

		auto settings = winrt::Windows::Storage::ApplicationData::Current().LocalSettings().Values();
		Volume(winrt::unbox_value_or<int>(settings.TryLookup(L"GlobalVolume"), 100));
		SongVolume(winrt::unbox_value_or<int>(settings.TryLookup(L"SongVolume"), 100));
		HitsoundVolume(winrt::unbox_value_or<int>(settings.TryLookup(L"HitsoundVolume"), 100));

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
		m_songPlayer.Pause();
		m_songPlayer.Source(nullptr);
		item.IsPlaying(true);
		m_currentItemToPlay = item;
		winrt::Windows::Media::Playback::MediaPlaybackItem playbackItem{songItemModel.Source()};
		co_await updateForSMTC(playbackItem);

		//Add timed metadata
		winrt::Windows::Media::Core::TimedMetadataTrack hitObjectsMeta{ L"0", L"en-US", winrt::Windows::Media::Core::TimedMetadataKind::Data };
		hitObjectsMeta.Label(L"Custom data track");
		hitObjectsMeta.CueEntered([this, &songItemModel](winrt::Windows::Media::Core::TimedMetadataTrack const& track, winrt::Windows::Media::Core::MediaCueEventArgs args) -> winrt::Windows::Foundation::IAsyncAction
		{
			auto hitObjectPtr = *reinterpret_cast<int*>(
				args
				.Cue()
				.as<winrt::Windows::Media::Core::DataCue>()
				.Data()
				.data());

			co_await m_hitSoundPlayer.playHitsound(*songItemModel.m_beatmaps[m_currentItemToPlay.SelectedVersionIndex()].originalFile->hitObjects[hitObjectPtr]);
		});

		
		//Maybe try using index instead of using pointers to hit object
		int i = 0;
		for (auto const& hitObject : songItemModel.m_beatmaps[item.SelectedVersionIndex()].originalFile->hitObjects)
		{
			winrt::Windows::Media::Core::DataCue cue;
			cue.StartTime(std::chrono::milliseconds{ hitObject->time });
			cue.Duration(std::chrono::milliseconds{ 100 });
			auto hitObjectRawPtr = hitObject.get();
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
		auto& recentInstance = Model::Recent::GetInstance();
		recentInstance.addRecordForSong(songItemModel);
		recentInstance.updateJumpList();
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
		return m_mod;
	}

	void PlayerViewModel::Mod(PlayMod mod)
	{
		if (mod != m_mod)
		{
			m_mod = mod;
			raisePropertyChange(L"ModString");
			raisePropertyChange(L"IsModEnabled");
		}
	}

	bool PlayerViewModel::IsModEnabled()
	{
		return m_mod != PlayMod::Normal;
	}

	void PlayerViewModel::IsModEnabled(bool)
	{
		//this property should not be set
		assert(false);
	}

	winrt::hstring PlayerViewModel::ModString()
	{
		switch (m_mod)
		{
			case PlayMod::HalfTime:		return L"HalfTime";
			case PlayMod::DoubleTime:	return L"DoubleTime";
			case PlayMod::NightCore:	return L"NightCore";
			default:					return L"NoMod";
		}
	}


	bool PlayerViewModel::UseSkinHitsound()
	{
		return false;
	}

	void PlayerViewModel::UseSkinHitsound(bool useSkinHitsound)
	{
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
		return static_cast<int>(m_songPlayer.Volume() * 100.0);
	}

	void PlayerViewModel::Volume(int volume)
	{
		m_songPlayer.Volume(static_cast<double>(volume) / 100.0);
		m_hitSoundPlayer.Volume(volume);
		raisePropertyChange(L"Volume");
	}

	int PlayerViewModel::SongVolume()
	{
		return 0;
	}

	void PlayerViewModel::SongVolume(int songVolume)
	{
	}

	int PlayerViewModel::HitsoundVolume()
	{
		return 0;
	}

	void PlayerViewModel::HitsoundVolume(int hitsoundVolume)
	{
	}

	void PlayerViewModel::Mute()
	{
		if (m_muteInfo.isMute)
		{
			m_muteInfo.songVolumeBefore = m_songPlayer.Volume();
			m_muteInfo.hitsoundVolumeBefore = m_hitSoundPlayer.Volume();
			m_muteInfo.globalVolumeBefore = Volume();
			Volume(0);
			m_muteInfo.isMute = true;
		}
		else
		{
			m_songPlayer.Volume(m_muteInfo.songVolumeBefore);
			m_hitSoundPlayer.Volume(m_muteInfo.hitsoundVolumeBefore);
			Volume(m_muteInfo.globalVolumeBefore);
			m_muteInfo.isMute = false;
		}
	}

	void PlayerViewModel::Save()
	{
		auto settings = winrt::Windows::Storage::ApplicationData::Current().LocalSettings().Values();
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
