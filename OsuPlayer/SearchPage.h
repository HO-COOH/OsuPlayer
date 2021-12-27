#pragma once

#include "SearchPage.g.h"
#include "SearchPageViewModel.g.h"

namespace winrt::OsuPlayer::implementation
{
    struct SearchPage : SearchPageT<SearchPage>
    {
        SearchPage();

        void OnNavigatedTo(winrt::Windows::UI::Xaml::Navigation::NavigationEventArgs args);

        OsuPlayer::SearchPageViewModel ViewModel();
    private:
        OsuPlayer::SearchPageViewModel m_model;
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct SearchPage : SearchPageT<SearchPage, implementation::SearchPage>
    {
    };
}
