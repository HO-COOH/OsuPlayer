#include "pch.h"
#include "Settings.h"
#if __has_include("Settings.g.cpp")
#include "Settings.g.cpp"
#endif
#include "ThemeHelper.h"
#include "winrt/Windows.Storage.Pickers.h"


using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::OsuPlayer::implementation
{
    Settings::Settings()
    {
        InitializeComponent();
    }

    winrt::Windows::Foundation::Collections::IObservableVector<winrt::OsuPlayer::OsuPathItem> Settings::osuPath()
    {
        return m_osuPath;
    }
}


void winrt::OsuPlayer::implementation::Settings::LightButton_Checked(
    winrt::Windows::Foundation::IInspectable const& sender, 
    winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
    //if(auto&& window = Window::Current().Content().try_as<FrameworkElement>(); window)
    //{
    //    window.RequestedTheme(ElementTheme::Light);
    //}
    ThemeHelper::RootTheme(winrt::Windows::UI::Xaml::ElementTheme::Light);
}


void winrt::OsuPlayer::implementation::Settings::DarkButton_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
    ThemeHelper::RootTheme(winrt::Windows::UI::Xaml::ElementTheme::Dark);
}


void winrt::OsuPlayer::implementation::Settings::SystemThemeButton_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{

}


winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Storage::StorageFolder> winrt::OsuPlayer::implementation::Settings::DoPickOsuFolder()
{
    winrt::Windows::Storage::Pickers::FolderPicker picker;
    picker.ViewMode(winrt::Windows::Storage::Pickers::PickerViewMode::List);

    auto folder = co_await picker.PickSingleFolderAsync();
    if (folder)
    {
        folder.GetFolderAsync(L"songs");
        co_return folder;
    }
    else
        co_return winrt::Windows::Storage::StorageFolder{nullptr};
}

winrt::Windows::Foundation::Collections::IObservableVector<winrt::OsuPlayer::OsuPathItem> winrt::OsuPlayer::implementation::Settings::m_osuPath = winrt::single_threaded_observable_vector< winrt::OsuPlayer::OsuPathItem>();

/*winrt::Windows::Foundation::IAsyncAction*/ void winrt::OsuPlayer::implementation::Settings::ListBox_SelectionChanged(
    winrt::Windows::Foundation::IInspectable const& sender,
    winrt::Windows::UI::Xaml::Controls::SelectionChangedEventArgs const& e
)
{
    //if (OsuPathListBox().SelectedIndex() == -1)
    //    return;
    //try {
    //    auto item = *e.AddedItems().First();
    //    if (item.as<winrt::hstring>() == L"+")
    //    {
    //        //Open file selection dialog
    //        do
    //        {
    //            try
    //            {
    //                auto result = co_await DoPickOsuFolder();
    //                auto&& path = result.Path();
    //                if (result && std::find_if(m_osuPath.begin(), m_osuPath.end(), [&path](winrt::OsuPlayer::OsuPathItem const& osuPathItem) { return osuPathItem.OsuPathText() == path; }) == m_osuPath.end())
    //                {
    //                    //TODO:
    //                    m_osuPath.InsertAt(0, winrt::OsuPlayer::OsuPathItem{});
    //                }
    //                break;
    //            }
    //            catch (winrt::hresult_error const& error)
    //            {
    //                continue;
    //            }
    //        } while (true);

    //    }
    //}
    //catch (winrt::hresult_error const& e)
    //{
    //    auto msg = e.message();
    //}
    //OsuPathListBox().SelectedIndex(-1); //reset the selectedIndex to -1 so this event can be fired again if the user clicked any items
    //model.setPath(m_osuPath.GetAt(0).OsuPathText());
    //auto vec = co_await model.readSomeAsync(20);
    //for (auto item : vec)
    //{
    //    m_songItems.Append(SongItem{});
    //}
}

MyMusicModel winrt::OsuPlayer::implementation::Settings::model;
winrt::Windows::Foundation::Collections::IObservableVector<winrt::OsuPlayer::SongItem> winrt::OsuPlayer::implementation::Settings::m_songItems = winrt::single_threaded_observable_vector< winrt::OsuPlayer::SongItem>();
winrt::event<winrt::OsuPlayer::OsuPathChangedHandler> winrt::OsuPlayer::implementation::Settings::s_handlers;

winrt::event_token winrt::OsuPlayer::implementation::Settings::s_osuPathChangedHandler(winrt::OsuPlayer::OsuPathChangedHandler const& handler)
{
    auto token = s_handlers.add(handler);
    s_handlers(m_osuPath);
    return token;
}

void winrt::OsuPlayer::implementation::Settings::s_osuPathChangedHandler(winrt::event_token const& token) noexcept
{
    s_handlers.remove(token);
}

winrt::Windows::Foundation::IAsyncAction winrt::OsuPlayer::implementation::Settings::AddOsuPathButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{

    //Open file selection dialog
    do
    {
        try
        {
            auto result = co_await DoPickOsuFolder();
            auto&& path = result.Path();
            if (result && 
                (m_osuPath == nullptr 
                    || (m_osuPath && (std::find_if(m_osuPath.begin(), m_osuPath.end(), [&path](winrt::OsuPlayer::OsuPathItem const& osuPathItem){ return osuPathItem.OsuPathText() == path; })) == m_osuPath.end())
                    )
                )
            {
                //TODO:
                winrt::OsuPlayer::OsuPathItem newItem;
                newItem.OsuPathText(path);
                m_osuPath.InsertAt(0, newItem);
            }
            break;
        }
        catch (winrt::hresult_error const& error)
        {
            continue;
        }
    } while (true);

    co_await model.setPath(m_osuPath.GetAt(0).OsuPathText());
    auto items = co_await model.readSomeAsync(20);
    for (auto item : items)
    {
        m_songItems.Append(item);
    }
}


void winrt::OsuPlayer::implementation::Settings::ClearAllButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{

}


void winrt::OsuPlayer::implementation::Settings::StoreAllCheckbox_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{

}


void winrt::OsuPlayer::implementation::Settings::StoreAllCheckbox_Unchecked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{

}


void winrt::OsuPlayer::implementation::Settings::StoreAllCheckbox_Indeterminate(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{

}


void winrt::OsuPlayer::implementation::Settings::DefaultHitsoundButton_Click(winrt::Microsoft::UI::Xaml::Controls::SplitButton const& sender, winrt::Microsoft::UI::Xaml::Controls::SplitButtonClickEventArgs const& args)
{

}

void winrt::OsuPlayer::implementation::Settings::HalftimeCheckbox_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
}

void winrt::OsuPlayer::implementation::Settings::DoubleTimeCheckbox_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
}

void winrt::OsuPlayer::implementation::Settings::NightCoreCheckbox_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
}
