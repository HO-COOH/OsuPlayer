#pragma once
#include "MyMusic.g.h"
#include "Settings.g.h"
#include <future>
#include "MyMusicModel.h"

namespace winrt::OsuPlayer::implementation
{
    struct MyMusic : MyMusicT<MyMusic>
    {
        
        MyMusic();

        void Category_ItemInvoked(
            winrt::Microsoft::UI::Xaml::Controls::NavigationView const& sender,
            winrt::Microsoft::UI::Xaml::Controls::NavigationViewItemInvokedEventArgs const& args
        );

        void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs const& e);
    private:
        std::future<void> onAddOsuPath;
        MyMusicModel model;
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct MyMusic : MyMusicT<MyMusic, implementation::MyMusic>
    {
    };
}
