#pragma once
#include <winrt/Windows.Foundation.Collections.h>
#include "SearchPageViewModel.g.h"
#include "SongItem.g.h"
#include "SearchModel.h"
#include <functional>

namespace winrt::OsuPlayer::implementation
{
    struct SearchPageViewModel : SearchPageViewModelT<SearchPageViewModel>
    {
        SearchPageViewModel() = default;


        winrt::hstring Keyword();
        void Keyword(winrt::hstring keyword);

        winrt::hstring SearchInfo();

        winrt::Windows::Foundation::Collections::IObservableVector<SongItem> SearchResult();
    private:
        winrt::hstring m_keyword;
        SearchModel m_model;
        std::function<bool()> m_onSearchStateChangedHandler;
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct SearchPageViewModel : SearchPageViewModelT<SearchPageViewModel, implementation::SearchPageViewModel>
    {
    };
}
