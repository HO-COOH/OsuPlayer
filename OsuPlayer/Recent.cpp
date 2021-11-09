#include "pch.h"
#include "Recent.h"
#if __has_include("Recent.g.cpp")
#include "Recent.g.cpp"
#endif

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::OsuPlayer::implementation
{
    Recent::Recent()
    {
        InitializeComponent();
    }

    int32_t Recent::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void Recent::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void Recent::ClickHandler(IInspectable const&, RoutedEventArgs const&)
    {
        Button().Content(box_value(L"Clicked"));
    }
}
