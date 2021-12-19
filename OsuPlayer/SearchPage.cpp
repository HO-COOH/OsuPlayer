#include "pch.h"
#include "SearchPage.h"
#if __has_include("SearchPage.g.cpp")
#include "SearchPage.g.cpp"
#endif

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::OsuPlayer::implementation
{
    SearchPage::SearchPage()
    {
        InitializeComponent();
    }

    void SearchPage::OnNavigatedTo(winrt::Windows::UI::Xaml::Navigation::NavigationEventArgs args)
    {
        m_keyword = args.Parameter().as<winrt::hstring>();
    }

}
