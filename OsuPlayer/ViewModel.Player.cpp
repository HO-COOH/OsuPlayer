#include "pch.h"
#include "ViewModel.Player.h"
#if __has_include("ViewModel.PlayerViewModel.g.cpp")
#include "ViewModel.PlayerViewModel.g.cpp"
#endif

#include "Model.MyMusic.h"
#include "Utils.h"

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
				raisePropertyChange(L"ProgressString");
			}
		);

	}
	winrt::Windows::Foundation::IAsyncAction PlayerViewModel::Play(ViewModel::SongItemViewModel item)
	{
		auto& songItemModel = *reinterpret_cast<SongItemModel*>(winrt::unbox_value<size_t>(item.ModelPointer()));
		co_await songItemModel.fillDataAsync();
		co_await songItemModel.Source().OpenAsync();
		m_songPlayer.Pause();
		m_songPlayer.Source(songItemModel.Source());
		m_songPlayer.Play();
		m_currentItemToPlay = item;
		raisePropertyChange(L"SongLength");
		raisePropertyChange(L"SongLengthString");
	}
	void PlayerViewModel::Play()
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
	winrt::hstring PlayerViewModel::ProgressString()
	{
		return Utils::GetDurationString(Utils::GetDuration(m_progress));
	}
	winrt::hstring PlayerViewModel::SongLengthString()
	{
		if (m_currentItemToPlay)
			return m_currentItemToPlay.LengthString();
		else
			return L"0:00";
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
	winrt::Windows::UI::Xaml::Media::ImageSource PlayerViewModel::ImageSource()
	{
		return winrt::Windows::UI::Xaml::Media::ImageSource{ nullptr };
	}
}
