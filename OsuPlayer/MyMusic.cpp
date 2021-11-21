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
        songItems = Settings::s_songItems();
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
