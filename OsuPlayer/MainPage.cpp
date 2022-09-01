#include "pch.h"

#include "MainPage.h"
#include "Model.MyMusic.h"

#if __has_include("MainPage.g.cpp")
#include "MainPage.g.cpp"
#endif
#include "ViewModelLocator.h"
#include <winrt/Windows.ApplicationModel.Resources.h>

using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace Model;

namespace winrt::OsuPlayer::implementation
{
    MainPage::MainPage()
    {
        InitializeComponent();

        ViewModelLocator::Current().Collections().VectorChanged([this](
                winrt::Windows::Foundation::Collections::IObservableVector<ViewModel::CollectionItem> sender,
                winrt::Windows::Foundation::Collections::IVectorChangedEventArgs args)
            {

            }
        );

        MyMusicModel::GetInstance().onIndexingFinished(
            [this](std::vector<SongItemModel> const& songs)
            {
                MyMusicInfoBadge().Value(songs.size());
                CollectionsInfoBadge().Value(MyMusicModel::GetInstance().m_collections.size());

                auto collectionNavigationItem = CollectionNavigationItem();
                collectionNavigationItem.MenuItems();
                auto children = collectionNavigationItem.MenuItems();
                for (auto collection : MyMusicModel::GetInstance().m_collections)
                {
                    winrt::Microsoft::UI::Xaml::Controls::NavigationViewItem item;
                    item.Content(winrt::box_value(winrt::to_hstring(collection.m_name)));
                    item.Tag(winrt::box_value(L"CollectionItemPage"));
                    item.Icon(winrt::Windows::UI::Xaml::Controls::SymbolIcon(winrt::Windows::UI::Xaml::Controls::Symbol::List));
                    winrt::Microsoft::UI::Xaml::Controls::InfoBadge badge;
                    badge.Value(collection.m_beatmapPtr.size());
                    item.InfoBadge(badge);
                    children.Append(item);
                }
                OutputDebugString(winrt::to_hstring(children.Size()).data());
            }
        );

        ViewModelLocator::Current().MyMusicViewModel().ListName(
            winrt::Windows::ApplicationModel::Resources::ResourceLoader::GetForCurrentView().GetString(L"MyMusicTitle")
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


    void MainPage::CollectionNavigationItem_PointerPressed(
        winrt::Windows::Foundation::IInspectable const& sender, 
        winrt::Windows::UI::Xaml::Input::PointerRoutedEventArgs const& e)
    {
        ContentFrame().Navigate(xaml_typename<OsuPlayer::CollectionView>());
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

            //Collections Page
            if (winrt::unbox_value_or<winrt::hstring>(invoked.Tag(), L"") == L"CollectionItemPage")
            {
                auto collectionPageViewModel = ViewModelLocator::Current().getCollectionPageByName(winrt::unbox_value<winrt::hstring>(invoked.Content()));
                MyMusic collectionPage;
                collectionPage.ViewModel(collectionPageViewModel);
                frame.Content(collectionPage);
            }
        }
    }
    ViewModel::MyMusicViewModel MainPage::MyMusicViewModel()
    {
        return ViewModelLocator::Current().MyMusicViewModel();
    }

    void MainPage::AddCollectionButton_Click(
        [[maybe_unused]]winrt::Windows::Foundation::IInspectable const& sender, 
        [[maybe_unused]]winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        winrt::Microsoft::UI::Xaml::Controls::NavigationViewItem newCollectionItem;
        winrt::Windows::UI::Xaml::Controls::TextBox nameInputBox;
        auto navigationMenuItems = NavigationViewControl().MenuItems();
        nameInputBox.Width(CollectionNavigationItem().Width());
        nameInputBox.KeyDown(
            [this](winrt::Windows::Foundation::IInspectable sender, winrt::Windows::UI::Xaml::Input::KeyRoutedEventArgs e)
            {
                if (e.Key() != winrt::Windows::System::VirtualKey::Enter)
                    return;

                auto inputBox = sender.as<winrt::Windows::UI::Xaml::Controls::TextBox>();
                winrt::Microsoft::UI::Xaml::Controls::NavigationViewItem subItem;
                subItem.Content(winrt::box_value(inputBox.Text()));
                subItem.ContextFlyout(
                    Resources()
                    .Lookup(winrt::box_value(L"CollectionMenu"))
                    .as<winrt::Windows::UI::Xaml::Controls::MenuFlyout>()
                );
                CollectionNavigationItem().MenuItems().Append(subItem);
                CollectionNavigationItem().IsExpanded(true);
                CollectionsInfoBadge().Value(CollectionsInfoBadge().Value() + 1);
                NavigationViewControl().MenuItems().RemoveAtEnd();
            }
        );
        newCollectionItem.Content(nameInputBox);
        navigationMenuItems.Append(newCollectionItem);
    }

    winrt::Windows::Foundation::IAsyncAction MainPage::CollectionPlayMenuItem_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        co_await ViewModelLocator::Current().getCollectionPageByName(
            winrt::unbox_value<winrt::hstring>(sender.as<winrt::Microsoft::UI::Xaml::Controls::NavigationViewItem>().Content())
        ).PlayCollection();
    }

}
