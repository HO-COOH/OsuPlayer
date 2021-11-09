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

    int32_t MyMusic::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void MyMusic::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void MyMusic::ClickHandler(IInspectable const&, RoutedEventArgs const&)
    {
    }
}


void winrt::OsuPlayer::implementation::MyMusic::Category_ItemInvoked(winrt::Microsoft::UI::Xaml::Controls::NavigationView const& sender, winrt::Microsoft::UI::Xaml::Controls::NavigationViewItemInvokedEventArgs const& args)
{

    if (args.InvokedItemContainer().IsSelected())
        return;

}
