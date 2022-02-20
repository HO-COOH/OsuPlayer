#include "pch.h"
#include "MyMusic.h"
#if __has_include("MyMusic.g.cpp")
#include "MyMusic.g.cpp"
#endif
#include "Utils.h"
#include "ViewModelLocator.h"

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::OsuPlayer::implementation
{
    MyMusic::MyMusic()
    {
        InitializeComponent();            
    }

    void MyMusic::OnSongItemEvent(SongItem item)
    {
        auto mainPage = Utils::FindParent<MainPage>(*this);
        
        mainPage.player().ViewModel().Play(item);
    }

    void MyMusic::OrderToggleButton_Click(
        winrt::Windows::Foundation::IInspectable const& sender, 
        winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
    }


    void MyMusic::SortOrderSelectionCombobox_SelectionChanged(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Windows::UI::Xaml::Controls::SelectionChangedEventArgs const& e)
    {
    }

    void MyMusic::Category_SelectionChanged(winrt::Microsoft::UI::Xaml::Controls::NavigationView const& sender, winrt::Microsoft::UI::Xaml::Controls::NavigationViewSelectionChangedEventArgs const& args)
    {
        auto&& invokedItem = args.SelectedItemContainer();
        auto&& name = invokedItem.Name();
        if (name == L"ArtistsCategoryItem")
        {
            MusicListFrame().Navigate(::xaml_typename<OsuPlayer::ArtistView>());
        }
        else if (name == L"CollectionsCategoryItem")
        {
            MusicListFrame().Navigate(::xaml_typename<OsuPlayer::CollectionView>());
        }
    }

    OsuPlayer::MyMusicViewModel MyMusic::ViewModel()
    {
        return ViewModelLocator::Current().MyMusicViewModel();
    }


    //void MyMusic::Category_ItemInvoked(winrt::Microsoft::UI::Xaml::Controls::NavigationView const& sender, winrt::Microsoft::UI::Xaml::Controls::NavigationViewItemInvokedEventArgs const& args)
    //{
    //    auto&& invokedItem = args.InvokedItemContainer();
    //    if (invokedItem.IsSelected())
    //        return;

    //    if (invokedItem.Name() == L"ArtistsCategoryItem")
    //    {
    //        MusicListFrame().Navigate(::xaml_typename<OsuPlayer::ArtistView>());
    //    }

    //}



    void MyMusic::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs const& e)
    {

    }
}


