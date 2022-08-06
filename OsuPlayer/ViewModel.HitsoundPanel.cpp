#include "pch.h"
#include "ViewModel.HitsoundPanel.h"
#if __has_include("ViewModel.HitsoundPanelViewModel.g.cpp")
#include "ViewModel.HitsoundPanelViewModel.g.cpp"
#include "ViewModel.HitsoundSample.g.cpp"
#include "ViewModel.HitsoundSampleSet.g.cpp"
#endif

#include <ViewModelLocator.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Media.Core.h>

namespace winrt::OsuPlayer::ViewModel::implementation
{
	winrt::Windows::Foundation::IAsyncAction HitsoundSample::Play()
	{
		auto folder = ViewModelLocator::Current().SettingsViewModel().SelectedSkin();
		s_player.Pause();
		s_player.Source(nullptr);
		OutputDebugString(m_name.data());
		try
		{
			auto file = co_await folder.GetFileAsync(m_name + L".wav");
			s_player.Source(winrt::Windows::Media::Core::MediaSource::CreateFromStorageFile(file));
			s_player.Play();
		}
		catch (...)
		{
			//File not found
		}
	}
}
