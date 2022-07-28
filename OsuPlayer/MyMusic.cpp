#include "pch.h"
#include "MyMusic.h"
#if __has_include("MyMusic.g.cpp")
#include "MyMusic.g.cpp"
#endif
#include "Utils.h"
#include "ViewModelLocator.h"


#include <winrt/Windows.ApplicationModel.DataTransfer.h>
#include "ColumnHeaderSettingDialog.g.h"
#include <winrt/Windows.UI.Xaml.Media.Animation.h>

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::OsuPlayer::implementation
{
    MyMusic::MyMusic()
    {
        InitializeComponent(); 
        m_showList ? showMusicList() : showAlbumView();
    }

    void MyMusic::OnSongItemEvent(ViewModel::SongItemViewModel item)
    {
        auto mainPage = Utils::FindParent<MainPage>(*this);
        
        //mainPage.player().ViewModel().Play(item);
    }

    void MyMusic::OrderToggleButton_Click(
        winrt::Windows::Foundation::IInspectable const& sender, 
        winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
    }


    void MyMusic::Category_SelectionChanged(winrt::Microsoft::UI::Xaml::Controls::NavigationView const& sender, winrt::Microsoft::UI::Xaml::Controls::NavigationViewSelectionChangedEventArgs const& args)
    {
        auto&& invokedItem = args.SelectedItemContainer();
        auto&& name = invokedItem.Name();
        if (name == L"ArtistsCategoryItem")
        {
            MusicListFrame().Navigate(::xaml_typename<OsuPlayer::ArtistView>());
        }
        else if (name == L"CollectionsCategoryItem")
        {
            MusicListFrame().Navigate(::xaml_typename<OsuPlayer::CollectionView>());
        }
    }

    OsuPlayer::ViewModel::MyMusicViewModel MyMusic::ViewModel()
    {
        return ViewModelLocator::Current().MyMusicViewModel();
    }



    void MyMusic::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs const& e)
    {

    }

    winrt::Windows::Foundation::IAsyncAction MyMusic::ColumnHeaderSetting_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        winrt::Windows::UI::Xaml::Controls::ContentDialog settingDialog;
        settingDialog.Content(ColumnHeaderSettingDialog{});
        settingDialog.PrimaryButtonText(L"OK");
        settingDialog.Title(winrt::box_value(L"Column Settings"));
        settingDialog.DefaultButton(winrt::Windows::UI::Xaml::Controls::ContentDialogButton::Primary);
        settingDialog.CloseButtonText(L"Cancel");
        if (co_await settingDialog.ShowAsync() == winrt::Windows::UI::Xaml::Controls::ContentDialogResult::Primary)
            ViewModelLocator::Current().ColumnSettings().Save();
    }


    void MyMusic::StackPanel_PointerEntered(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Input::PointerRoutedEventArgs const& e)
    {
        auto stackPanel = sender.as<winrt::Windows::UI::Xaml::Controls::StackPanel>();
        auto grid = stackPanel.Children().GetAt(0).as<winrt::Windows::UI::Xaml::Controls::StackPanel>();
        auto subGrid = grid.Children().GetAt(1);
        subGrid.RenderTransform(winrt::Windows::UI::Xaml::Media::CompositeTransform{});
        //subGrid.RenderTransformOrigin(winrt::Windows::Foundation::Point{ 0.0,1.0 });
        auto height = subGrid.ActualSize().y;
        winrt::Windows::UI::Xaml::Media::Animation::Storyboard storyboard;
        winrt::Windows::UI::Xaml::Media::Animation::DoubleAnimation animation;
        animation.To(-height);
        animation.From(0.0);
        animation.Duration(winrt::Windows::UI::Xaml::DurationHelper::FromTimeSpan(std::chrono::milliseconds{ 150 }));
        storyboard.Duration(winrt::Windows::UI::Xaml::DurationHelper::FromTimeSpan(std::chrono::milliseconds{ 150 }));
        storyboard.Children().Append(animation);
        animation.EnableDependentAnimation(true);
        storyboard.SetTarget(animation, subGrid);
        storyboard.SetTargetProperty(animation, L"(UIElement.RenderTransform).(CompositeTransform.TranslateY)");
        storyboard.Begin();
    }


    void MyMusic::StackPanel_PointerExited(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Input::PointerRoutedEventArgs const& e)
    {
        auto stackPanel = sender.as<winrt::Windows::UI::Xaml::Controls::StackPanel>();
        auto grid = stackPanel.Children().GetAt(0).as<winrt::Windows::UI::Xaml::Controls::StackPanel>();
        auto subGrid = grid.Children().GetAt(1);
        subGrid.RenderTransform(winrt::Windows::UI::Xaml::Media::CompositeTransform{});
        //subGrid.RenderTransformOrigin(winrt::Windows::Foundation::Point{ 0.0,1.0 });
        auto height = subGrid.ActualSize().y;
        winrt::Windows::UI::Xaml::Media::Animation::Storyboard storyboard;
        winrt::Windows::UI::Xaml::Media::Animation::DoubleAnimation animation;
        animation.To(0.0);
        animation.From(-height);
        animation.Duration(winrt::Windows::UI::Xaml::DurationHelper::FromTimeSpan(std::chrono::milliseconds{ 150 }));
        storyboard.Duration(winrt::Windows::UI::Xaml::DurationHelper::FromTimeSpan(std::chrono::milliseconds{ 150 }));
        storyboard.Children().Append(animation);
        animation.EnableDependentAnimation(true);
        storyboard.SetTarget(animation, subGrid);
        storyboard.SetTargetProperty(animation, L"(UIElement.RenderTransform).(CompositeTransform.TranslateY)");
        storyboard.Begin();
    }

    void MyMusic::ListViewMode_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        m_showList = !m_showList;
        m_showList ? showMusicList() : showAlbumView();
    }

    void MyMusic::showMusicList()
    {
        ListIcon().Visibility(winrt::Windows::UI::Xaml::Visibility::Visible);
        AlbumIcon().Visibility(winrt::Windows::UI::Xaml::Visibility::Collapsed);

        MusicList().Visibility(winrt::Windows::UI::Xaml::Visibility::Visible);
        MusicAlbumView().Visibility(winrt::Windows::UI::Xaml::Visibility::Collapsed);
    }

    void MyMusic::showAlbumView()
    {
        ListIcon().Visibility(winrt::Windows::UI::Xaml::Visibility::Collapsed);
        AlbumIcon().Visibility(winrt::Windows::UI::Xaml::Visibility::Visible);

        MusicList().Visibility(winrt::Windows::UI::Xaml::Visibility::Collapsed);
        MusicAlbumView().Visibility(winrt::Windows::UI::Xaml::Visibility::Visible);
    }
}
