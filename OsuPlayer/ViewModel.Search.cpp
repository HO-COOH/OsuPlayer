#include "pch.h"
#include "ViewModel.Search.h"
#if __has_include("ViewModel.SearchPageViewModel.g.cpp")
#include "ViewModel.SearchPageViewModel.g.cpp"
#endif

namespace winrt::OsuPlayer::ViewModel::implementation
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

    winrt::Windows::Foundation::Collections::IObservableVector<ViewModel::SongItemViewModel> SearchPageViewModel::SearchResult()
    {
        //auto result = co_await m_model.doSearch(m_keyword);
        return winrt::single_threaded_observable_vector<ViewModel::SongItemViewModel>();
    }

}
