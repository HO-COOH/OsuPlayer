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

        winrt::Windows::Foundation::Collections::IObservableVector<winrt::hstring> data();
        void data(winrt::Windows::Foundation::Collections::IObservableVector<winrt::hstring> d) { m_data = d; }

        //These following 2 methods are for supporting property change events
        winrt::event_token PropertyChanged(winrt::Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(winrt::event_token const& token) noexcept;

        void Raise();
    private:
        winrt::hstring m_title;
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::hstring> m_data = winrt::single_threaded_observable_vector<winrt::hstring>();

        winrt::event<winrt::Windows::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;
    };

    struct ArtistView : ArtistViewT<ArtistView>
    {
        ArtistView();

        winrt::Windows::Foundation::Collections::IObservableVector<winrt::OsuPlayer::GroupedData> groups(); 
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::OsuPlayer::GroupedData> m_groups = winrt::single_threaded_observable_vector<winrt::OsuPlayer::GroupedData>();
        void GridView_GotFocus(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);

        void OnNavigatedTo(winrt::Windows::UI::Xaml::Navigation::NavigationEventArgs e);

        winrt::event_token PropertyChanged(winrt::Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(winrt::event_token const& token) noexcept;

    private:

        winrt::event<winrt::Windows::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;
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
