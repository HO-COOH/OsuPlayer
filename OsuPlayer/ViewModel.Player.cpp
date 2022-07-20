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
		m_model.m_songPlayer.PlaybackSession().PositionChanged(
			[this](winrt::Windows::Media::Playback::MediaPlaybackSession session, winrt::Windows::Foundation::IInspectable _)->winrt::Windows::Foundation::IAsyncAction
			{
				m_progress = session.Position().count() / 10'000ll;
				co_await winrt::resume_foreground(winrt::Windows::ApplicationModel::Core::CoreApplication::MainView().CoreWindow().Dispatcher());
				m_propertyChanged(*this, winrt::Windows::UI::Xaml::Data::PropertyChangedEventArgs{ L"Progress" });
				m_propertyChanged(*this, winrt::Windows::UI::Xaml::Data::PropertyChangedEventArgs{ L"ProgressString" });
			}
		);

	}
	void PlayerViewModel::Play(SongItem item)
	{
		auto const& songItemModel = MyMusicModel::GetInstance().get(item.ViewModel().Index());
		m_model.Source(songItemModel.Source());
		m_currentItemToPlay = item;
		m_propertyChanged(*this, winrt::Windows::UI::Xaml::Data::PropertyChangedEventArgs{ L"SongLength" });
		m_propertyChanged(*this, winrt::Windows::UI::Xaml::Data::PropertyChangedEventArgs{ L"SongLengthString" });
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
			return m_currentItemToPlay.ViewModel().LengthString();
		else
			return L"0:00";
	}
	int PlayerViewModel::Progress()
	{
		return m_progress;
	}
	int PlayerViewModel::SongLength()
	{
		return m_currentItemToPlay.ViewModel().Length();
	}
	int PlayerViewModel::Volume()
	{
		return m_model.m_songPlayer.Volume() * 100.0;
	}
	void PlayerViewModel::Volume(int volume)
	{
		m_model.m_songPlayer.Volume(static_cast<double>(volume) / 100.0);
		m_model.m_hitSoundPlayer.Volume(static_cast<double>(volume) / 100.0);
	}
	winrt::Windows::UI::Xaml::Media::ImageSource PlayerViewModel::ImageSource()
	{
		return winrt::Windows::UI::Xaml::Media::ImageSource{ nullptr };
	}
	winrt::event_token PlayerViewModel::PropertyChanged(winrt::Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
	{
		return m_propertyChanged.add(handler);
	}
	void PlayerViewModel::PropertyChanged(winrt::event_token const& token) noexcept
	{
		m_propertyChanged.remove(token);
	}
}
