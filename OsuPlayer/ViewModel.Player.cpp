#include "pch.h"
#include "ViewModel.Player.h"
#if __has_include("ViewModel.PlayerViewModel.g.cpp")
#include "ViewModel.PlayerViewModel.g.cpp"
#endif

#include "Model.MyMusic.h"
#include "Utils.h"
#include "Model.Recent.h"

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
		m_songPlayer.Pause();
		m_songPlayer.Source(nullptr);
		item.IsPlaying(true);
		m_currentItemToPlay = item;
		winrt::Windows::Media::Playback::MediaPlaybackItem playbackItem{songItemModel.Source()};
		co_await updateForSMTC(playbackItem);
		m_songPlayer.Source(playbackItem);
		m_songPlayer.Play();
		
		//update ui
		raisePropertyChange(L"SongLength");

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

	PlayMod PlayerViewModel::Mod()
	{
		return PlayMod();
	}

	void PlayerViewModel::Mod(PlayMod mod)
	{
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
		return m_songPlayer.Volume() * 100.0;
	}

	void PlayerViewModel::Volume(int volume)
	{
		m_songPlayer.Volume(static_cast<double>(volume) / 100.0);
		m_hitSoundPlayer.Volume(static_cast<double>(volume) / 100.0);
	}

	int implementation::PlayerViewModel::SongVolume()
	{
		return 0;
	}

	void implementation::PlayerViewModel::SongVolume(int songVolume)
	{
	}

	int implementation::PlayerViewModel::HitsoundVolume()
	{
		return 0;
	}

	void implementation::PlayerViewModel::HitsoundVolume(int hitsoundVolume)
	{
	}

	void implementation::PlayerViewModel::Mute()
	{
	}

	void implementation::PlayerViewModel::Save()
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

		auto stream = winrt::Windows::Storage::Streams::RandomAccessStreamReference::CreateFromFile(co_await m_currentItemToPlay.SongImageFile());
		//co_await stream.OpenReadAsync();
		properties.Thumbnail(stream);
		properties.Type(winrt::Windows::Media::MediaPlaybackType::Music);
		item.ApplyDisplayProperties(properties);
		co_return;
	}
	void PlayerViewModel::updateForSMTC()
	{
	}
}
