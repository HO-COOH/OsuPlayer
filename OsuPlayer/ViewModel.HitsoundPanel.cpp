﻿#include "pch.h"
#include "ViewModel.HitsoundPanel.h"
#if __has_include("ViewModel.HitsoundPanelViewModel.g.cpp")
#include "ViewModel.HitsoundPanelViewModel.g.cpp"
#include "ViewModel.HitsoundSample.g.cpp"
#include "ViewModel.HitsoundSampleSet.g.cpp"
#endif

#include <ViewModelLocator.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Storage.Pickers.h>
#include <winrt/Windows.Media.Core.h>
#include <Utils.Log.hpp>
#include <Utils.h>

namespace winrt::OsuPlayer::ViewModel::implementation
{
	HitsoundSample::HitsoundSample(winrt::hstring name) : 
		m_name{ name },
		m_volume{ winrt::unbox_value_or<int>(Utils::GetSettings().Values().TryLookup(m_name), 100)}
	{
	}

	HitsoundSampleSet::HitsoundSampleSet(winrt::hstring sampleSetName) :
		m_hitNormal{sampleSetName + L"-hitnormal"},
		m_hitFinish{sampleSetName + L"-hitfinish"},
		m_hitWhistle{sampleSetName + L"-hitwhistle"},
		m_hitClap{sampleSetName + L"-hitclap"}
	{
	}

	void HitsoundSample::Play()
	{
		s_player.Pause();
		s_player.Source(nullptr);
		s_player.Volume(static_cast<double>(m_volume) / 100.0);
		if (!m_source)
			m_source = winrt::Windows::Media::Core::MediaSource::CreateFromStorageFile(m_hitsoundFile);
		s_player.Source(m_source);
		s_player.Play();
	}

	void HitsoundSample::Save()
	{
		assert(!m_name.empty());
		Utils::GetSettings().Values().Insert(m_name, winrt::box_value(m_volume));
	}

	winrt::Windows::Foundation::IAsyncAction HitsoundSample::Update()
	{
		try
		{
			m_hitsoundFile = co_await ViewModelLocator::Current().SettingsViewModel().SelectedSkin().GetFileAsync(m_name + L".wav");
		}
		catch (winrt::hresult_error const& e)
		{
			Utils::ConsoleLogger{} << ViewModelLocator::Current().SettingsViewModel().SelectedSkin().Path().data() << L"\\" << m_name.data() << L" not found\n";
			constexpr auto FileNotFoundErrorCode = HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
			
			if (e.code() == FileNotFoundErrorCode)
				m_hitsoundFile = nullptr;
			else
				throw;
		}
		raisePropertyChange(L"IsAvailable");
	}

	winrt::Windows::Foundation::IAsyncAction HitsoundSample::Replace()
	{
		winrt::Windows::Storage::Pickers::FileOpenPicker picker;
		picker.FileTypeFilter().Append(L"*");
		auto newFile = co_await picker.PickSingleFileAsync();
		if (newFile == nullptr)
			co_return;

		m_hitsoundFile = newFile;
	}

	winrt::Windows::Media::Playback::IMediaPlaybackSource HitsoundSample::Source()
	{
		if (!m_source)
			m_source = winrt::Windows::Media::Core::MediaSource::CreateFromStorageFile(m_hitsoundFile);
		return m_source;
	}

	winrt::Windows::Foundation::IAsyncAction HitsoundPanelViewModel::Update()
	{
		co_await winrt::when_all(
			m_normal.Hitnormal(). Update(),
			m_normal.Hitfinish(). Update(),
			m_normal.Hitwhistle().Update(),
			m_normal.Hitclap().   Update(),

			m_soft.Hitnormal().Update(),
			m_soft.Hitfinish().Update(),
			m_soft.Hitwhistle().Update(),
			m_soft.Hitclap().Update(),

			m_drum.Hitnormal().Update(),
			m_drum.Hitfinish().Update(),
			m_drum.Hitwhistle().Update(),
			m_drum.Hitclap().Update()
		);
	}
}
