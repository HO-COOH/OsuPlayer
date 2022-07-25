﻿#include "pch.h"
#include "MyMusic.h"
#if __has_include("MyMusic.g.cpp")
#include "MyMusic.g.cpp"
#endif
#include "Utils.h"
#include "ViewModelLocator.h"


#include <winrt/Windows.ApplicationModel.DataTransfer.h>
#include "ColumnHeaderSettingDialog.g.h"

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::OsuPlayer::implementation
{
    MyMusic::MyMusic()
    {
        InitializeComponent();            
    }

    void MyMusic::OnSongItemEvent(ViewModel::SongItemViewModel item)
    {
        auto mainPage = Utils::FindParent<MainPage>(*this);
        
        //mainPage.player().ViewModel().Play(item);
    }

    void MyMusic::OrderToggleButton_Click(
        winrt::Windows::Foundation::IInspectable const& sender, 
        winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
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

    OsuPlayer::ViewModel::MyMusicViewModel MyMusic::ViewModel()
    {
        return ViewModelLocator::Current().MyMusicViewModel();
    }



    void MyMusic::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs const& e)
    {

    }

    winrt::Windows::Foundation::IAsyncAction MyMusic::ColumnHeaderSetting_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        winrt::Windows::UI::Xaml::Controls::ContentDialog settingDialog;
        settingDialog.Content(ColumnHeaderSettingDialog{});
        settingDialog.PrimaryButtonText(L"OK");
        settingDialog.Title(winrt::box_value(L"Column Settings"));
        settingDialog.DefaultButton(winrt::Windows::UI::Xaml::Controls::ContentDialogButton::Primary);
        settingDialog.CloseButtonText(L"Cancel");
        if (co_await settingDialog.ShowAsync() == winrt::Windows::UI::Xaml::Controls::ContentDialogResult::Primary)
            ViewModelLocator::Current().ColumnSettings().Save();
    }
}
