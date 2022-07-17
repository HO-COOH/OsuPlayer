#include "pch.h"
#include "OsuPathItem.h"
#if __has_include("OsuPathItem.g.cpp")
#include "OsuPathItem.g.cpp"
#endif

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::OsuPlayer::implementation
{
    OsuPathItem::OsuPathItem()
    {
        InitializeComponent();
    }

    OsuPathItem::OsuPathItem(ViewModel::OsuPathItemViewModel viewModel)
        : m_model{ viewModel }
    {
        InitializeComponent();
    }

    ViewModel::OsuPathItemViewModel OsuPathItem::ViewModel()
    {
        return m_model;
    }
}


void winrt::OsuPlayer::implementation::OsuPathItem::DeleteButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{

}
