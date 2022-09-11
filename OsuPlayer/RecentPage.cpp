#include "pch.h"
#include "RecentPage.h"
#if __has_include("RecentPage.g.cpp")
#include "RecentPage.g.cpp"
#endif

#include "ViewModelLocator.h"

namespace winrt::OsuPlayer::implementation
{
    RecentPage::RecentPage()
    {
        InitializeComponent();
    }

    ViewModel::RecentViewModel RecentPage::ViewModel()
    {
        return ViewModelLocator::Current().RecentViewModel();
    }

}
