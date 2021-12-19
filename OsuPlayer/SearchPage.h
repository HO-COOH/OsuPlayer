#pragma once

#include "SearchPage.g.h"

namespace winrt::OsuPlayer::implementation
{
    struct SearchPage : SearchPageT<SearchPage>
    {
        SearchPage();

        winrt::hstring Keyword() { return std::wstring{L"Results for \""} + m_keyword + L"\""; }

        void OnNavigatedTo(winrt::Windows::UI::Xaml::Navigation::NavigationEventArgs args);
    private:
        winrt::hstring m_keyword;
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct SearchPage : SearchPageT<SearchPage, implementation::SearchPage>
    {
    };
}
