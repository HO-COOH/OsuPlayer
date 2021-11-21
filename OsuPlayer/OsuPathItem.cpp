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

    bool OsuPathItem::IsLoading()
    {
        return true;
    }

    winrt::hstring OsuPathItem::OsuPathText()
    {
        return m_osuPathText;
    }

    void OsuPathItem::OsuPathText(winrt::hstring const& value)
    {
        m_osuPathText = value;
    }

    winrt::hstring OsuPathItem::BeatmapCountText()
    {
        return L"12,345 beatmaps";
    }


}


void winrt::OsuPlayer::implementation::OsuPathItem::DeleteButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{

}
