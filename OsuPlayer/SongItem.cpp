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

    SongItem::SongItem(OsuPlayer::SongItemViewModel viewModel) : m_model{viewModel}
    {
        InitializeComponent();
    }

    OsuPlayer::SongItemViewModel SongItem::ViewModel()
    {
        return m_model;
    }

    void SongItem::Grid_DoubleTapped(
        winrt::Windows::Foundation::IInspectable const& sender, 
        winrt::Windows::UI::Xaml::Input::DoubleTappedRoutedEventArgs const& e)
    {
        m_model.PlayCurrent();
    }


    void SongItem::CheckBox_Checked(
        winrt::Windows::Foundation::IInspectable const& sender, 
        winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {

    }


    void SongItem::CheckBox_Unchecked(
        winrt::Windows::Foundation::IInspectable const& sender, 
        winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {

    }
}


void winrt::OsuPlayer::implementation::SongItem::PropertyMenuItem_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{

}
