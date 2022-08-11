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
#include <winrt/Windows.ApplicationModel.Resources.h>

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
        [[maybe_unused]]winrt::Windows::Foundation::IInspectable const& sender, 
        [[maybe_unused]]winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
    }


    void MyMusic::Category_SelectionChanged(
        [[maybe_unused]]winrt::Microsoft::UI::Xaml::Controls::NavigationView const& sender, 
        winrt::Microsoft::UI::Xaml::Controls::NavigationViewSelectionChangedEventArgs const& args)
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

    void MyMusic::OnNavigatedTo([[maybe_unused]]Windows::UI::Xaml::Navigation::NavigationEventArgs const& e)
    {

    }

    winrt::Windows::Foundation::IAsyncAction MyMusic::ColumnHeaderSetting_Click(
        [[maybe_unused]]winrt::Windows::Foundation::IInspectable const& sender, 
        [[maybe_unused]]winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        winrt::Windows::UI::Xaml::Controls::ContentDialog settingDialog;
        settingDialog.Content(ColumnHeaderSettingDialog{});
        settingDialog.PrimaryButtonText(L"OK");
        settingDialog.Title(winrt::box_value(winrt::Windows::ApplicationModel::Resources::ResourceLoader::GetForCurrentView().GetString(L"ColumnHeaderSetting/Text")));
        settingDialog.DefaultButton(winrt::Windows::UI::Xaml::Controls::ContentDialogButton::Primary);
        settingDialog.CloseButtonText(L"Cancel");
        if (co_await settingDialog.ShowAsync() == winrt::Windows::UI::Xaml::Controls::ContentDialogResult::Primary)
            ViewModelLocator::Current().ColumnSettings().Save();
    }


    void MyMusic::StackPanel_PointerEntered(
        winrt::Windows::Foundation::IInspectable const& sender, 
        [[maybe_unused]]winrt::Windows::UI::Xaml::Input::PointerRoutedEventArgs const& e)
    {
        auto stackPanel = sender.as<winrt::Windows::UI::Xaml::Controls::StackPanel>();
        auto grid = stackPanel.Children().GetAt(0).as<winrt::Windows::UI::Xaml::Controls::StackPanel>();
        auto subGrid = grid.Children().GetAt(1);
        ShowAlbumViewInfo(subGrid.as<winrt::Windows::UI::Xaml::Controls::Grid>());
    }

    void MyMusic::StackPanel_PointerExited(
        winrt::Windows::Foundation::IInspectable const& sender, 
        [[maybe_unused]]winrt::Windows::UI::Xaml::Input::PointerRoutedEventArgs const& e)
    {
        auto stackPanel = sender.as<winrt::Windows::UI::Xaml::Controls::StackPanel>();
        auto grid = stackPanel.Children().GetAt(0).as<winrt::Windows::UI::Xaml::Controls::StackPanel>();
        auto subGrid = grid.Children().GetAt(1);
        HideAlbumViewInfo(subGrid.as<winrt::Windows::UI::Xaml::Controls::Grid>());
    }

    void MyMusic::ListViewMode_Click(
        winrt::Windows::Foundation::IInspectable const& sender, 
        [[maybe_unused]]winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        m_showList = !m_showList;
        m_showList ? showMusicList() : showAlbumView();
    }

    void MyMusic::showMusicList()
    {
        ListIcon().Visibility(winrt::Windows::UI::Xaml::Visibility::Visible);
        AlbumIcon().Visibility(winrt::Windows::UI::Xaml::Visibility::Collapsed);
        MusicListHeaderGrid().Visibility(winrt::Windows::UI::Xaml::Visibility::Visible);

        UnloadObject(MusicAlbumView());
        FindName(L"MusicList");
    }

    void MyMusic::showAlbumView()
    {
        ListIcon().Visibility(winrt::Windows::UI::Xaml::Visibility::Collapsed);
        AlbumIcon().Visibility(winrt::Windows::UI::Xaml::Visibility::Visible);
        MusicListHeaderGrid().Visibility(winrt::Windows::UI::Xaml::Visibility::Collapsed);

        UnloadObject(MusicList());
        FindName(L"MusicAlbumView");
    }

    void MyMusic::ShowAlbumViewInfo(winrt::Windows::UI::Xaml::Controls::Grid subGrid, bool isForEvent)
    {
        if (!isForEvent && subGrid.DataContext().as<ViewModel::SongItemViewModel>().IsPlaying())
            return;

        subGrid.RenderTransform(winrt::Windows::UI::Xaml::Media::CompositeTransform{});
        auto height = subGrid.ActualSize().y;
        winrt::Windows::UI::Xaml::Media::Animation::Storyboard storyboard;
        winrt::Windows::UI::Xaml::Media::Animation::DoubleAnimation animation;
        animation.From(0.0);
        animation.To(-height);
        animation.Duration(winrt::Windows::UI::Xaml::DurationHelper::FromTimeSpan(std::chrono::milliseconds{ 150 }));
        storyboard.Duration(winrt::Windows::UI::Xaml::DurationHelper::FromTimeSpan(std::chrono::milliseconds{ 150 }));
        storyboard.Children().Append(animation);
        animation.EnableDependentAnimation(true);
        storyboard.SetTarget(animation, subGrid);
        storyboard.SetTargetProperty(animation, L"(UIElement.RenderTransform).(CompositeTransform.TranslateY)");
        storyboard.Begin();
    }

    void MyMusic::HideAlbumViewInfo(winrt::Windows::UI::Xaml::Controls::Grid subGrid, bool isForEvent)
    {
        if (!isForEvent && subGrid.DataContext().as<ViewModel::SongItemViewModel>().IsPlaying())
            return;
        subGrid.RenderTransform(winrt::Windows::UI::Xaml::Media::CompositeTransform{});
        auto height = subGrid.ActualSize().y;
        winrt::Windows::UI::Xaml::Media::Animation::Storyboard storyboard;
        winrt::Windows::UI::Xaml::Media::Animation::DoubleAnimation animation;
        animation.From(-height);
        animation.To(0.0);
        animation.Duration(winrt::Windows::UI::Xaml::DurationHelper::FromTimeSpan(std::chrono::milliseconds{ 150 }));
        storyboard.Duration(winrt::Windows::UI::Xaml::DurationHelper::FromTimeSpan(std::chrono::milliseconds{ 150 }));
        storyboard.Children().Append(animation);
        animation.EnableDependentAnimation(true);
        storyboard.SetTarget(animation, subGrid);
        storyboard.SetTargetProperty(animation, L"(UIElement.RenderTransform).(CompositeTransform.TranslateY)");
        storyboard.Begin();
    }

    void MyMusic::SongImage_ImageFailed(
        winrt::Windows::Foundation::IInspectable const& sender, 
        winrt::Windows::UI::Xaml::ExceptionRoutedEventArgs const& e)
    {

    }

    void winrt::OsuPlayer::implementation::MyMusic::InfoGrid_Loaded(
        winrt::Windows::Foundation::IInspectable const& sender, 
        [[maybe_unused]]winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        auto infoGrid = sender.as<winrt::Windows::UI::Xaml::Controls::Grid>();
        auto item = infoGrid.DataContext().as<ViewModel::SongItemViewModel>();
        m_events.emplace_back(item.IsPlayingChanged(
            winrt::auto_revoke,
            [infoGrid](auto, bool isPlaying)
            {
                isPlaying ? ShowAlbumViewInfo(infoGrid, true) : HideAlbumViewInfo(infoGrid, true);
            }
        ));
    }

    void MyMusic::MusicListGrid_Loaded(
        winrt::Windows::Foundation::IInspectable const& sender, 
        [[maybe_unused]]winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        auto infoGrid = sender
            .as<winrt::Windows::UI::Xaml::Controls::Grid>()
            .Children()
            .GetAt(1)
            .as<winrt::Windows::UI::Xaml::Controls::Grid>();
        auto infoGridChildren = infoGrid.Children();

        m_events.emplace_back(infoGrid.DataContext().as<ViewModel::SongItemViewModel>().IsPlayingChanged(
            winrt::auto_revoke,
            [infoGridChildren, this](auto, bool isPlaying)
            {
                for (int i = 0; i < 4; ++i)
                {
                    infoGridChildren.GetAt(i).as<winrt::Windows::UI::Xaml::Controls::TextBlock>().Foreground(
                        winrt::Windows::UI::Xaml::Media::SolidColorBrush{
                            Resources().Lookup(
                                isPlaying ?
                                winrt::box_value(L"SystemAccentColor") :
                                winrt::box_value(L"SystemBaseHighColor")
                            ).as<winrt::Windows::UI::Color>()
                        }
                    );
                }
            }
        ));
    }

}
