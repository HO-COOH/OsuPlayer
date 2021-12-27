#include "pch.h"
#include "PlayerViewModel.h"
#if __has_include("PlayerViewModel.g.cpp")
#include "PlayerViewModel.g.cpp"
#endif

namespace winrt::OsuPlayer::implementation
{
	void PlayerViewModel::Play(SongItem item)
	{
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
	int PlayerViewModel::Volume()
	{
		return 0;
	}
	void PlayerViewModel::Volume(int volume)
	{
	}
	winrt::Windows::UI::Xaml::Media::ImageSource PlayerViewModel::ImageSource()
	{
		return winrt::Windows::UI::Xaml::Media::ImageSource{ nullptr };
	}
}
