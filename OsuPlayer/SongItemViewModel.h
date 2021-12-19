#pragma once

#include "SongItemViewModel.g.h"
#include "SongItemModel.h"
#include <winrt/Windows.Media.Core.h>
#include <winrt/Windows.UI.Xaml.Media.Imaging.h>

namespace winrt::OsuPlayer::implementation
{
    struct SongItemViewModel : SongItemViewModelT<SongItemViewModel>
    {
        SongItemViewModel() = default;


        winrt::hstring SongName();
        winrt::hstring Singer();
        winrt::hstring Mapper();
        winrt::hstring Length();

        winrt::Windows::Foundation::Collections::IObservableVector<winrt::hstring> Versions();


        winrt::Windows::Media::Core::MediaSource SongFile();


        winrt::Windows::UI::Xaml::Media::Imaging::BitmapImage Image();

        void PlayCurrent();

        //These following 2 methods are for supporting property change events
        winrt::event_token PropertyChanged(winrt::Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(winrt::event_token const& token) noexcept;

    private:
        SongItemModel m_model;

        //This is for supporting property change events
        winrt::event<winrt::Windows::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct SongItemViewModel : SongItemViewModelT<SongItemViewModel, implementation::SongItemViewModel>
    {
    };
}
