#pragma once

#include "ArtistView.g.h"
#include "GroupedData.g.h"
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.UI.Xaml.Controls.h>

namespace winrt::OsuPlayer::implementation
{
    struct GroupedData : GroupedDataT<GroupedData>
    {
        GroupedData() = default;

        winrt::hstring Title();
        void Title(winrt::hstring title);

        winrt::Windows::Foundation::Collections::IObservableVector<winrt::Windows::UI::Xaml::Controls::Button> data();
    private:
        winrt::hstring m_title;
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::Windows::UI::Xaml::Controls::Button> m_data = winrt::single_threaded_observable_vector<winrt::Windows::UI::Xaml::Controls::Button>();
    };

    struct ArtistView : ArtistViewT<ArtistView>
    {
        ArtistView();

        winrt::Windows::Foundation::Collections::IObservableVector<winrt::OsuPlayer::GroupedData> groups(); 
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::OsuPlayer::GroupedData> m_groups = winrt::single_threaded_observable_vector<winrt::OsuPlayer::GroupedData>();
        void GridView_GotFocus(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct ArtistView : ArtistViewT<ArtistView, implementation::ArtistView>
    {
    };

    struct GroupedData : GroupedDataT<GroupedData, implementation::GroupedData>
    {

    };
}
