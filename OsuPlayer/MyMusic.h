#pragma once
#include "MyMusic.g.h"
#include "ViewModel.MyMusicViewModel.g.h"
#include "SongItem.g.h"

namespace winrt::OsuPlayer::implementation
{
    struct MyMusic : MyMusicT<MyMusic>
    {
        
        MyMusic();

        //void Category_ItemInvoked(
        //    winrt::Microsoft::UI::Xaml::Controls::NavigationView const& sender,
        //    winrt::Microsoft::UI::Xaml::Controls::NavigationViewItemInvokedEventArgs const& args
        //);

        void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs const& e);

        void OnSongItemEvent(SongItem item);

        void OrderToggleButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void Category_SelectionChanged(winrt::Microsoft::UI::Xaml::Controls::NavigationView const& sender, winrt::Microsoft::UI::Xaml::Controls::NavigationViewSelectionChangedEventArgs const& args);
        
        ViewModel::MyMusicViewModel ViewModel();
    private:
        //OsuPlayer::MyMusicViewModel m_model;

    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct MyMusic : MyMusicT<MyMusic, implementation::MyMusic>
    {
    };
}
