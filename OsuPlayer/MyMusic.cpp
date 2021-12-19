#include "pch.h"
#include "MyMusic.h"
#if __has_include("MyMusic.g.cpp")
#include "MyMusic.g.cpp"
#endif

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::OsuPlayer::implementation
{
    MyMusic::MyMusic()
    {
        InitializeComponent();            
    }
}


void winrt::OsuPlayer::implementation::MyMusic::Category_ItemInvoked(winrt::Microsoft::UI::Xaml::Controls::NavigationView const& sender, winrt::Microsoft::UI::Xaml::Controls::NavigationViewItemInvokedEventArgs const& args)
{
    if (args.InvokedItemContainer().IsSelected())
        return;
}



void winrt::OsuPlayer::implementation::MyMusic::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs const& e)
{

}

#include "Utils.h"
//void winrt::OsuPlayer::implementation::MyMusic::OnSongItemEvent(winrt::Windows::Storage::StorageFile file)
//{
//    auto mainPage = Utils::FindParent<MainPage>(*this);
//    mainPage.player().Play(file);
//}

void winrt::OsuPlayer::implementation::MyMusic::OnSongItemEvent(SongItem item)
{
    auto mainPage = Utils::FindParent<MainPage>(*this);
    mainPage.player().Play(item);
}


void winrt::OsuPlayer::implementation::MyMusic::OrderToggleButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{

}


void winrt::OsuPlayer::implementation::MyMusic::SortOrderSelectionCombobox_SelectionChanged(
    winrt::Windows::Foundation::IInspectable const& sender, 
    winrt::Windows::UI::Xaml::Controls::SelectionChangedEventArgs const& e)
{
}

OsuPlayer::MyMusicViewModel winrt::OsuPlayer::implementation::MyMusic::ViewModel()
{
    return m_model;
}
