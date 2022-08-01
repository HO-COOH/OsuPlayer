#include "pch.h"

#include "MainPage.h"
#include "Model.MyMusic.h"

#if __has_include("MainPage.g.cpp")
#include "MainPage.g.cpp"
#endif
#include "ViewModelLocator.h"

using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace Model;

namespace winrt::OsuPlayer::implementation
{
    MainPage::MainPage()
    {
        InitializeComponent();

        MyMusicViewModel().Collections().VectorChanged([this](
            winrt::Windows::Foundation::Collections::IObservableVector<ViewModel::CollectionItem> sender,
            winrt::Windows::Foundation::Collections::IVectorChangedEventArgs args)
            {

            }
        );

        MyMusicModel::GetInstance().onIndexingFinished(
            [this](std::vector<SongItemModel> const& songs)
            {
                //m_navigationItems.GetAt(0).Value(songs.size());
                //m_navigationItems.GetAt(2).Value(MyMusicModel::GetInstance().m_collections.size());
                MyMusicInfoBadge().Value(songs.size());
                CollectionsInfoBadge().Value(MyMusicModel::GetInstance().m_collections.size());

                auto collectionNavigationItem = CollectionNavigationItem();
                collectionNavigationItem.MenuItems();
                auto children = collectionNavigationItem.MenuItems();
                for (auto collection : MyMusicModel::GetInstance().m_collections)
                {
                    winrt::Microsoft::UI::Xaml::Controls::NavigationViewItem item;
                    item.Content(winrt::box_value(winrt::to_hstring(L"CollectionItem")));
                    item.Tag(winrt::box_value(winrt::to_hstring(L"CollectionItem")));
                    item.Icon(winrt::Windows::UI::Xaml::Controls::SymbolIcon(winrt::Windows::UI::Xaml::Controls::Symbol::List));
                    children.Append(item);
                }
                if (children.Size() > 1)
                    children.RemoveAt(0);
                OutputDebugString(winrt::to_hstring(children.Size()).data());
            }
        );
    }

    void MainPage::SearchBox_QuerySubmitted(
        winrt::Windows::UI::Xaml::Controls::AutoSuggestBox const& sender, 
        winrt::Windows::UI::Xaml::Controls::AutoSuggestBoxQuerySubmittedEventArgs const& args)
    {
        auto text = args.QueryText();

        ContentFrame().Navigate(xaml_typename<OsuPlayer::SearchPage>(), winrt::box_value(text));
    }


    void MainPage::NavigationViewItem_PointerPressed(
        winrt::Windows::Foundation::IInspectable const& sender, 
        winrt::Windows::UI::Xaml::Input::PointerRoutedEventArgs const& e)
    {
        ContentFrame().Navigate(::xaml_typename<OsuPlayer::MyMusic>());
    }


    void MainPage::NavigationViewControl_ItemInvoked(
        winrt::Microsoft::UI::Xaml::Controls::NavigationView const& sender,
        winrt::Microsoft::UI::Xaml::Controls::NavigationViewItemInvokedEventArgs const& args)
    {

        if (args.InvokedItemContainer().IsSelected())
            return;

        auto const& frame = ContentFrame();
        if (args.IsSettingsInvoked())
        {
            if (frame.CurrentSourcePageType() != winrt::xaml_typename<Settings>())
                frame.Navigate(winrt::xaml_typename<Settings>());
        }
        else
        {
            OutputDebugString(winrt::get_class_name(args.InvokedItemContainer()).data());
            auto&& invoked = args.InvokedItemContainer().as<winrt::Microsoft::UI::Xaml::Controls::NavigationViewItem>();
            //other navigation items here
        }
    }
    ViewModel::MyMusicViewModel MainPage::MyMusicViewModel()
    {
        return ViewModelLocator::Current().MyMusicViewModel();
    }
}
