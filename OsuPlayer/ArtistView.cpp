#include "pch.h"
#include "ArtistView.h"
#if __has_include("ArtistView.g.cpp")
#include "ArtistView.g.cpp"
#endif

#include "GroupedData.g.cpp"
#include "Utils.Log.hpp"
#include <future>
using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace Utils;
namespace winrt::OsuPlayer::implementation
{
    ArtistView::ArtistView()
    {
        InitializeComponent();

        DataContext(*this);
    }


    winrt::Windows::Foundation::Collections::IObservableVector<winrt::OsuPlayer::GroupedData> ArtistView::groups()
    {
        for (auto group : m_groups)
            ConsoleLogger{} << group.data().Size() << '\t';
        ConsoleLogger{} << '\n';
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

    winrt::Windows::Foundation::Collections::IObservableVector<winrt::hstring> GroupedData::data()
    {
        return m_data;
    }

    winrt::event_token GroupedData::PropertyChanged(winrt::Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
    {
        return m_propertyChanged.add(handler);

    }

    void GroupedData::PropertyChanged(winrt::event_token const& token) noexcept
    {
        m_propertyChanged.remove(token);
    }

    void GroupedData::Raise()
    {
        m_propertyChanged(*this, winrt::Windows::UI::Xaml::Data::PropertyChangedEventArgs{ L"data" });
    }

}


void winrt::OsuPlayer::implementation::ArtistView::GridView_GotFocus(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
    GridView().StartBringIntoView();
}

void winrt::OsuPlayer::implementation::ArtistView::OnNavigatedTo(winrt::Windows::UI::Xaml::Navigation::NavigationEventArgs e)
{
    winrt::OsuPlayer::GroupedData d1, d2;
    for (wchar_t i = 'A'; i <= 'z'; ++i)
    {
        d1.data().Append(winrt::to_hstring(i));
    }
    d1.Title(L"Group1");
    d2.Title(L"Group2");
    m_groups.Append(d1);
    m_groups.Append(d2);


    d1.Raise();
    d2.Raise();

    AlphabetGroups().Source(m_groups);

    auto groups = AlphabetGroups();
    auto view = groups.View();
    auto source = groups.Source().as<winrt::Windows::Foundation::Collections::IObservableVector<winrt::OsuPlayer::GroupedData>>();
    auto current = view.CurrentItem();

    ConsoleLogger{} <<"CollectionViewSource View: " << AlphabetGroups().View().Size() << '\n';
    ConsoleLogger{} << "CollectionViewSource View Source: " << AlphabetGroups().Source().as<winrt::Windows::Foundation::Collections::IObservableVector<winrt::OsuPlayer::GroupedData>>().Size() << '\n';
    ConsoleLogger{} << "CollectionViewSource View Source: " << source.GetAt(0).data().Size() << '\t' << source.GetAt(1).data().Size() << '\n';
    m_propertyChanged(*this, winrt::Windows::UI::Xaml::Data::PropertyChangedEventArgs{ L"AlphabetGroups" });
}

void winrt::OsuPlayer::implementation::ArtistView::PropertyChanged(winrt::event_token const& token) noexcept
{
    m_propertyChanged.remove(token);
}

winrt::event_token winrt::OsuPlayer::implementation::ArtistView::PropertyChanged(winrt::Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
{
    return m_propertyChanged.add(handler);

}