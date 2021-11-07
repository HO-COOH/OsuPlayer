#include "pch.h"
#include "Settings.h"
#if __has_include("Settings.g.cpp")
#include "Settings.g.cpp"
#endif
#include "ThemeHelper.h"

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
        return winrt::Windows::Foundation::Collections::IObservableVector<winrt::hstring>();
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
