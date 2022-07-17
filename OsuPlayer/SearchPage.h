#pragma once

#include "SearchPage.g.h"
#include "ViewModel.SearchPageViewModel.g.h"

namespace winrt::OsuPlayer::implementation
{
    struct SearchPage : SearchPageT<SearchPage>
    {
        SearchPage();

        void OnNavigatedTo(winrt::Windows::UI::Xaml::Navigation::NavigationEventArgs args);

        ViewModel::SearchPageViewModel ViewModel();
    private:
        ViewModel::SearchPageViewModel m_model;
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct SearchPage : SearchPageT<SearchPage, implementation::SearchPage>
    {
    };
}
