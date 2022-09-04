#pragma once

#include "ViewModel.SongItemViewModel.g.h"
#include "Model.SongItem.h"
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Media.Core.h>
#include <winrt/Windows.UI.Xaml.Media.Imaging.h>
#include "Utils.PropertyChangeHelper.hpp"

namespace winrt::OsuPlayer::ViewModel::implementation
{
    struct SongItemViewModel : SongItemViewModelT<SongItemViewModel>, Utils::PropertyChangeHelper<SongItemViewModel>
    {
        SongItemViewModel() = default;


        winrt::hstring SongName();
        winrt::hstring Singer();
        winrt::hstring Mapper();
        winrt::hstring LengthString();
        int Length();
        int SelectedVersionIndex();
        winrt::hstring SelectedVersion();

        winrt::Windows::Storage::StorageFolder Folder() { return getModel().getFolder(); }

        void SelectedVersionIndex(int index);

        void ModelPointer(winrt::Windows::Foundation::IInspectable modelPointer) { m_modelPointer = modelPointer; }
        winrt::Windows::Foundation::IInspectable ModelPointer() { return m_modelPointer; }
        

        winrt::Windows::Foundation::Collections::IObservableVector<winrt::hstring> Versions();
        bool IsPlaying() { return m_isPlaying; }
        void IsPlaying(bool isPlaying);

        winrt::event_token IsPlayingChanged(winrt::Windows::Foundation::EventHandler<bool> const& handler) { return m_isPlayingChanged.add(handler); }
        void IsPlayingChanged(winrt::event_token const& token) noexcept { m_isPlayingChanged.remove(token); }

        void PlayCurrent();

        winrt::Windows::Foundation::IAsyncAction ShowProperty();
        winrt::Windows::Foundation::IAsyncAction OpenFile();
        winrt::Windows::Foundation::IAsyncAction OpenFolder();

        winrt::Windows::UI::Xaml::Media::Imaging::BitmapImage SongImage();

        winrt::Windows::Foundation::IAsyncAction loadImage();

        winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Storage::StorageFile> SongImageFile();
        winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Storage::Streams::IRandomAccessStream> SongImageStream();
    private:
        winrt::Windows::Foundation::IAsyncAction getImageFile();
        winrt::Windows::UI::Xaml::Media::Imaging::BitmapImage m_imageStream;
        int m_index{};
        int m_versionIndex{};
        bool m_isPlaying = false;
        winrt::event<winrt::Windows::Foundation::EventHandler<bool>> m_isPlayingChanged;
        winrt::Windows::Foundation::IInspectable m_modelPointer;
        winrt::Windows::Storage::StorageFile m_imageFile{ nullptr };
        
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::hstring> m_versions = winrt::single_threaded_observable_vector<winrt::hstring>();
        Model::SongItemModel& getModel();
    };
}

namespace winrt::OsuPlayer::ViewModel::factory_implementation
{
    struct SongItemViewModel : SongItemViewModelT<SongItemViewModel, implementation::SongItemViewModel>
    {
    };
}
