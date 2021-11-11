﻿#include "pch.h"
#include "MainPage.h"
#include "MainPage.g.cpp"


using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::OsuPlayer::implementation
{
    MainPage::MainPage()
    {
        InitializeComponent();
    }

}


void winrt::OsuPlayer::implementation::MainPage::SearchBox_QuerySubmitted(winrt::Windows::UI::Xaml::Controls::AutoSuggestBox const& sender, winrt::Windows::UI::Xaml::Controls::AutoSuggestBoxQuerySubmittedEventArgs const& args)
{

}


void winrt::OsuPlayer::implementation::MainPage::NavigationViewItem_PointerPressed(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Input::PointerRoutedEventArgs const& e)
{
    ContentFrame().Navigate(::xaml_typename<OsuPlayer::MyMusic>());
}


void winrt::OsuPlayer::implementation::MainPage::NavigationViewControl_ItemInvoked(winrt::Microsoft::UI::Xaml::Controls::NavigationView const& sender, winrt::Microsoft::UI::Xaml::Controls::NavigationViewItemInvokedEventArgs const& args)
{

    if (args.InvokedItemContainer().IsSelected())
        return;

    auto const& frame = ContentFrame();
    if(args.IsSettingsInvoked())
    {
        if (frame.CurrentSourcePageType() != winrt::xaml_typename<Settings>())
            frame.Navigate(winrt::xaml_typename<Settings>());
    }
    else
    {
        auto&& invokedContainer = args.InvokedItemContainer();

    }
}