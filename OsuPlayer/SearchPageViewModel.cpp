#include "pch.h"
#include "SearchPageViewModel.h"
#if __has_include("SearchPageViewModel.g.cpp")
#include "SearchPageViewModel.g.cpp"
#endif

namespace winrt::OsuPlayer::implementation
{
    winrt::hstring SearchPageViewModel::Keyword()
    {
        return m_keyword;
    }

    void SearchPageViewModel::Keyword(winrt::hstring keyword)
    {
        m_keyword = keyword;
    }

    winrt::hstring SearchPageViewModel::SearchInfo()
    {
        return std::wstring{ L"Results for \"" } + m_keyword + L"\"";
    }

    winrt::Windows::Foundation::Collections::IObservableVector<SongItem> SearchPageViewModel::SearchResult()
    {
        //auto result = co_await m_model.doSearch(m_keyword);
        return winrt::single_threaded_observable_vector<SongItem>();
    }

}
