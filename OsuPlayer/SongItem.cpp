#include "pch.h"
#include "SongItem.h"
#if __has_include("SongItem.g.cpp")
#include "SongItem.g.cpp"
#endif

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::OsuPlayer::implementation
{
    SongItem::SongItem()
    {
        InitializeComponent();
    }

}


void winrt::OsuPlayer::implementation::SongItem::CheckBox_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{

}


void winrt::OsuPlayer::implementation::SongItem::CheckBox_Unchecked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{

}
