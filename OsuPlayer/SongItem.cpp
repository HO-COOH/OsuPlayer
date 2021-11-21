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

    SongItem::SongItem(winrt::hstring songName) : SongItem()
    {
        this->songName = songName;
    }

    SongItem::SongItem(winrt::hstring SongName, winrt::hstring Singer) : SongItem()
    {
        this->songName = SongName;
        this->singer = Singer;
    }

    SongItem::SongItem(winrt::hstring SongName, winrt::hstring Singer, int Length) : SongItem()
    {
        this->songName = SongName;
        this->singer = Singer;
        this->length = Length;
    }

    SongItem::SongItem(winrt::hstring SongName, winrt::hstring Singer, int Length, winrt::hstring Mapper) : SongItem()
    {
        this->songName = SongName;
        this->singer = Singer;
        this->mapper = Mapper;
        this->length = Length;
    }

    
}


winrt::hstring winrt::OsuPlayer::implementation::SongItem::Length()
{
    auto const secondsTotal = length / 1'000;
    auto const minutes = secondsTotal / 60;
    auto const seconds = secondsTotal - 60 * minutes;
    return winrt::hstring{ std::to_wstring(minutes) + L":" + std::to_wstring(seconds) };
}

void winrt::OsuPlayer::implementation::SongItem::CheckBox_Checked(
    winrt::Windows::Foundation::IInspectable const& sender, 
    winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{

}


void winrt::OsuPlayer::implementation::SongItem::CheckBox_Unchecked(
    winrt::Windows::Foundation::IInspectable const& sender,
    winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{

}
