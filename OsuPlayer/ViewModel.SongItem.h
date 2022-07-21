#pragma once

#include "ViewModel.SongItemViewModel.g.h"
#include "Model.SongItem.h"
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Media.Core.h>
#include <winrt/Windows.UI.Xaml.Media.Imaging.h>

namespace winrt::OsuPlayer::ViewModel::implementation
{
    struct SongItemViewModel : SongItemViewModelT<SongItemViewModel>
    {
        SongItemViewModel() = default;


        winrt::hstring SongName();
        winrt::hstring Singer();
        winrt::hstring Mapper();
        winrt::hstring LengthString();
        int Length();
        int Index();
        int SelectedVersionIndex();


        void SongName(winrt::hstring songName);
        void Singer(winrt::hstring singer);
        void Mapper(winrt::hstring mapper);
        void Length(int length);
        void Index(int index);
        void SelectedVersionIndex(int index);

        void ModelPointer(winrt::Windows::Foundation::IInspectable modelPointer) { m_modelPointer = modelPointer; }
        winrt::Windows::Foundation::IInspectable ModelPointer() { return m_modelPointer; }
        

        winrt::Windows::Foundation::Collections::IObservableVector<winrt::hstring> Versions();


        //winrt::Windows::Media::Core::MediaSource SongFile();


        //winrt::Windows::UI::Xaml::Media::Imaging::BitmapImage Image();


        void PlayCurrent();

        //These following 2 methods are for supporting property change events
        winrt::event_token PropertyChanged(winrt::Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(winrt::event_token const& token) noexcept;

    private:
        winrt::hstring m_songName;
        winrt::hstring m_singer;
        winrt::hstring m_mapper;
        int m_length;
        int m_index;
        int m_versionIndex = 0;
        winrt::Windows::Foundation::IInspectable m_modelPointer;
        
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::hstring> m_versions = winrt::single_threaded_observable_vector<winrt::hstring>();
        //SongItemModel m_model;

        //This is for supporting property change events
        winrt::event<winrt::Windows::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;
    };
}

namespace winrt::OsuPlayer::ViewModel::factory_implementation
{
    struct SongItemViewModel : SongItemViewModelT<SongItemViewModel, implementation::SongItemViewModel>
    {
    };
}
