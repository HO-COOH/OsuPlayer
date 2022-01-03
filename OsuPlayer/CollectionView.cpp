#include "pch.h"
#include "CollectionView.h"
#if __has_include("CollectionView.g.cpp")
#include "CollectionView.g.cpp"
#endif

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::OsuPlayer::implementation
{
    CollectionView::CollectionView()
    {
        InitializeComponent();
    }

    int32_t CollectionView::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void CollectionView::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void CollectionView::ClickHandler(IInspectable const&, RoutedEventArgs const&)
    {
        Button().Content(box_value(L"Clicked"));
    }
}
