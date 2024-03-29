﻿#pragma once

#include "MainPage.g.h"
#include "Player.g.h"
#include <winrt/Windows.UI.Xaml.Input.h>

#include "ViewModel.MyMusicViewModel.g.h"

namespace winrt::OsuPlayer::implementation
{
    struct MainPage : MainPageT<MainPage>
    {
        MainPage();
        void SearchBox_QuerySubmitted(
            winrt::Windows::UI::Xaml::Controls::AutoSuggestBox const& sender, 
            winrt::Windows::UI::Xaml::Controls::AutoSuggestBoxQuerySubmittedEventArgs const& args);

        void NavigationViewItem_PointerPressed(
            winrt::Windows::Foundation::IInspectable const& sender, 
            winrt::Windows::UI::Xaml::Input::PointerRoutedEventArgs const& e);

        void NavigationViewControl_ItemInvoked(
            winrt::Microsoft::UI::Xaml::Controls::NavigationView const& sender, 
            winrt::Microsoft::UI::Xaml::Controls::NavigationViewItemInvokedEventArgs const& args);

        Player player() { return {}; }

        ViewModel::MyMusicViewModel MyMusicViewModel();
        void CollectionNavigationItem_PointerPressed(
            winrt::Windows::Foundation::IInspectable const& sender, 
            winrt::Windows::UI::Xaml::Input::PointerRoutedEventArgs const& e);

        void AddCollectionButton_Click(
            winrt::Windows::Foundation::IInspectable const& sender, 
            winrt::Windows::UI::Xaml::RoutedEventArgs const& e);

        winrt::Windows::Foundation::IAsyncAction CollectionPlayMenuItem_Click(
            winrt::Windows::Foundation::IInspectable const& sender, 
            winrt::Windows::UI::Xaml::RoutedEventArgs const& e);

        void RecentNavigationItem_PointerPressed(
            winrt::Windows::Foundation::IInspectable const& sender, 
            winrt::Windows::UI::Xaml::Input::PointerRoutedEventArgs const& e);

        static Microsoft::UI::Xaml::Controls::WebView2 Browser() { return s_browser; }
    private:
        static winrt::Microsoft::UI::Xaml::Controls::WebView2 s_browser;
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct MainPage : MainPageT<MainPage, implementation::MainPage>
    {
    };
}
