#pragma once
#include "MyMusic.g.h"
#include "ViewModel.MyMusicViewModel.g.h"
#include "Utils.PropertyChangeHelper.hpp"

namespace winrt::OsuPlayer::implementation
{
    struct MyMusic : MyMusicT<MyMusic>, Utils::PropertyChangeHelper<MyMusic>
    {
        
        MyMusic();

        //void Category_ItemInvoked(
        //    winrt::Microsoft::UI::Xaml::Controls::NavigationView const& sender,
        //    winrt::Microsoft::UI::Xaml::Controls::NavigationViewItemInvokedEventArgs const& args
        //);

        void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs const& e);

        void OnSongItemEvent(ViewModel::SongItemViewModel item);

        void OrderToggleButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void Category_SelectionChanged(winrt::Microsoft::UI::Xaml::Controls::NavigationView const& sender, winrt::Microsoft::UI::Xaml::Controls::NavigationViewSelectionChangedEventArgs const& args);
        
        ViewModel::MyMusicViewModel ViewModel();
        winrt::Windows::Foundation::IAsyncAction ColumnHeaderSetting_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void StackPanel_PointerEntered(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Input::PointerRoutedEventArgs const& e);
        void StackPanel_PointerExited(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Input::PointerRoutedEventArgs const& e);
        void ListViewMode_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);

    private:
        void showMusicList();
        void showAlbumView();
        bool m_showList = true;
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct MyMusic : MyMusicT<MyMusic, implementation::MyMusic>
    {
    };
}
