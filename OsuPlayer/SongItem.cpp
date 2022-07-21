#include "pch.h"
#include "SongItem.h"
#if __has_include("SongItem.g.cpp")
#include "SongItem.g.cpp"
#endif

#include "SongItemDialog.g.h"
#include "MyMusic.g.h"
#include "Utils.h"
#include "ViewModelLocator.h"

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::OsuPlayer::implementation
{
    SongItem::SongItem()
    {
        InitializeComponent();
    }

    SongItem::SongItem(OsuPlayer::ViewModel::SongItemViewModel viewModel) : m_model{viewModel}
    {
        InitializeComponent();
    }

    OsuPlayer::ViewModel::SongItemViewModel SongItem::ViewModel()
    {
        return m_model;
    }

    void SongItem::Grid_DoubleTapped(
        winrt::Windows::Foundation::IInspectable const& sender, 
        winrt::Windows::UI::Xaml::Input::DoubleTappedRoutedEventArgs const& e)
    {
        auto myMusicView = Utils::FindParent<OsuPlayer::MyMusic>(*this);
        myMusicView.OnSongItemEvent(*this);
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

    winrt::Windows::Foundation::IAsyncAction winrt::OsuPlayer::implementation::SongItem::PropertyMenuItem_Click(
        winrt::Windows::Foundation::IInspectable const& sender, 
        winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        //auto myMusicView = Utils::FindParent<OsuPlayer::MyMusic>(*this);
        //co_await myMusicView.ViewModel().ShowPropertyOf(m_model.Index(), m_model.SelectedVersionIndex());

        auto myMusicViewModel = ViewModelLocator::Current().MyMusicViewModel();
        co_await myMusicViewModel.ShowPropertyOf(ViewModel());
    }

}
