#include "pch.h"
#include "ArtistView.h"
#if __has_include("ArtistView.g.cpp")
#include "ArtistView.g.cpp"
#endif

#include "GroupedData.g.cpp"
#include "Log.h"
using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::OsuPlayer::implementation
{
    ArtistView::ArtistView()
    {
        InitializeComponent();

        winrt::OsuPlayer::GroupedData d1;
        for (wchar_t i = 'A'; i <= 'z'; ++i)
        {
            winrt::Windows::UI::Xaml::Controls::Button button;
            std::wstring s;
            s += i;
            button.Content(winrt::box_value(winrt::hstring{ s }));
            d1.data().Append(button);
        }
        d1.Title(L"Group1");
        m_groups.Append(d1);
        m_groups.Append({});
    }


    winrt::Windows::Foundation::Collections::IObservableVector<winrt::OsuPlayer::GroupedData> ArtistView::groups()
    {

        return m_groups;
    }

    winrt::hstring GroupedData::Title()
    {
        return m_title;
    }

    void GroupedData::Title(winrt::hstring title)
    {
        m_title = title;
    }

    winrt::Windows::Foundation::Collections::IObservableVector<winrt::Windows::UI::Xaml::Controls::Button> GroupedData::data()
    {
        //auto items = winrt::single_threaded_observable_vector<winrt::Windows::UI::Xaml::Controls::Button>();

        //for (wchar_t i = 'A'; i <= 'z'; ++i)
        //{
        //    winrt::Windows::UI::Xaml::Controls::Button button;
        //    std::wstring s;
        //    s += i;
        //    button.Content(winrt::box_value(winrt::hstring{ s }));
        //    m_data.Append(button);
        //}
        ConsoleLogger{} << m_data.Size() << '\n';
        return m_data;
    }

}


void winrt::OsuPlayer::implementation::ArtistView::GridView_GotFocus(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
    GridView().StartBringIntoView();
}
