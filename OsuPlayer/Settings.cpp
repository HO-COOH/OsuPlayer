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

    winrt::Windows::Foundation::Collections::IObservableVector<winrt::hstring> Settings::osuPath()
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

winrt::Windows::Foundation::Collections::IObservableVector<winrt::hstring> winrt::OsuPlayer::implementation::Settings::m_osuPath 
    = [] { auto v = winrt::single_threaded_observable_vector<winrt::hstring>(); v.Append(L"+"); return v; }();

winrt::Windows::Foundation::IAsyncAction winrt::OsuPlayer::implementation::Settings::ListBox_SelectionChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Controls::SelectionChangedEventArgs const& e)
{
    if (OsuPathListBox().SelectedIndex() == -1)
        return;
    try {
        auto item = *e.AddedItems().First();
        if (item.as<winrt::hstring>() == L"+")
        {
            //Open file selection dialog
            do
            {
                try
                {
                    auto result = co_await DoPickOsuFolder();
                    auto&& path = result.Path();
                    if(result && std::find(m_osuPath.begin(), m_osuPath.end(), path) == m_osuPath.end())
                        m_osuPath.InsertAt(0, std::move(path));
                    break;
                }
                catch (winrt::hresult_error const& error)
                {
                    continue;
                }
            } while (true);
        }
    }
    catch (winrt::hresult_error const& e)
    {
        auto msg = e.message();
    }
    OsuPathListBox().SelectedIndex(-1); //reset the selectedIndex to -1 so this event can be fired again if the user clicked any items
}
