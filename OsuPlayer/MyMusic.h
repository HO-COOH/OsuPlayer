#pragma once
#include "MyMusic.g.h"
#include "Settings.g.h"

namespace winrt::OsuPlayer::implementation
{
    struct MyMusic : MyMusicT<MyMusic>
    {
        
        MyMusic();

        int32_t MyProperty();
        void MyProperty(int32_t value);

        void ClickHandler(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args);
        void Category_ItemInvoked(winrt::Microsoft::UI::Xaml::Controls::NavigationView const& sender, winrt::Microsoft::UI::Xaml::Controls::NavigationViewItemInvokedEventArgs const& args);
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct MyMusic : MyMusicT<MyMusic, implementation::MyMusic>
    {
    };
}
