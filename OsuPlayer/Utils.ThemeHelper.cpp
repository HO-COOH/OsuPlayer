#include "pch.h"
#include "Utils.ThemeHelper.h"
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.UI.ViewManagement.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.h>
#include "App.xaml.g.h"

namespace winrt
{
    hstring to_hstring(winrt::Windows::UI::Xaml::ElementTheme theme)
    {
        switch (theme)
        {
            case winrt::Windows::UI::Xaml::ElementTheme::Default:   return L"Default";
            case winrt::Windows::UI::Xaml::ElementTheme::Light:     return L"Light";
            case winrt::Windows::UI::Xaml::ElementTheme::Dark:      return L"Dark";
        }
    }
}

namespace Utils
{

    winrt::Windows::UI::Xaml::ElementTheme ThemeHelper::ActualTheme()
    {
        if (auto&& rootElement = winrt::Windows::UI::Xaml::Window::Current().Content().try_as<winrt::Windows::UI::Xaml::FrameworkElement>(); rootElement)
        {
            if (auto&& requestedTheme = rootElement.RequestedTheme(); requestedTheme != winrt::Windows::UI::Xaml::ElementTheme::Default)
                return requestedTheme;
        }
        return winrt::Windows::UI::Xaml::ElementTheme::Default;
    }

    winrt::Windows::UI::Xaml::ElementTheme ThemeHelper::RootTheme()
    {
        if (auto&& rootElement = winrt::Windows::UI::Xaml::Window::Current().Content().try_as<winrt::Windows::UI::Xaml::FrameworkElement>(); rootElement)
        {
            return rootElement.RequestedTheme();
        }
        return winrt::Windows::UI::Xaml::ElementTheme::Default;
    }

    void ThemeHelper::RootTheme(winrt::Windows::UI::Xaml::ElementTheme theme)
    {
        if (auto&& rootElement = winrt::Windows::UI::Xaml::Window::Current().Content().try_as<winrt::Windows::UI::Xaml::FrameworkElement>(); rootElement)
            rootElement.RequestedTheme(theme);

        winrt::Windows::Storage::ApplicationData::Current().LocalSettings().Values().Insert(SelectedAppThemeKey, winrt::box_value(winrt::to_hstring(theme)));

        UpdateTitlebarColors();
    }

    void ThemeHelper::Initialize()
    {
        CurrentApplicationWindow = winrt::Windows::UI::Xaml::Window::Current();

        if (auto savedTheme = winrt::Windows::Storage::ApplicationData::Current().LocalSettings().Values().TryLookup(SelectedAppThemeKey); savedTheme)
        {
            auto str = winrt::unbox_value_or<winrt::hstring>(savedTheme, L"");
            if (str == L"Default")
                RootTheme(winrt::Windows::UI::Xaml::ElementTheme::Default);
            else if (str == L"Light")
                RootTheme(winrt::Windows::UI::Xaml::ElementTheme::Light);
            else if (str == L"Dark")
                RootTheme(winrt::Windows::UI::Xaml::ElementTheme::Dark);
        }

        uiSettingsEventRevoker = uiSettings.ColorValuesChanged(winrt::auto_revoke, &UiSettings_ColorValuesChanged);
    }

    bool ThemeHelper::IsDarkTheme()
    {
        if (RootTheme() == winrt::Windows::UI::Xaml::ElementTheme::Default)
            return winrt::Windows::UI::Xaml::Application::Current().RequestedTheme() == winrt::Windows::UI::Xaml::ApplicationTheme::Dark;
        return RootTheme() == winrt::Windows::UI::Xaml::ElementTheme::Dark;
    }

    void ThemeHelper::UpdateTitlebarColors()
    {
        auto&& titleBar = winrt::Windows::UI::ViewManagement::ApplicationView::GetForCurrentView().TitleBar();
        titleBar.ButtonForegroundColor(IsDarkTheme() ? winrt::Windows::UI::Colors::White() : winrt::Windows::UI::Colors::Black());
    }

    void ThemeHelper::UiSettings_ColorValuesChanged(
        [[maybe_unused]]winrt::Windows::UI::ViewManagement::UISettings sender, 
        [[maybe_unused]]winrt::Windows::Foundation::IInspectable const& args)
    {
        if (CurrentApplicationWindow)
        {
            CurrentApplicationWindow.Dispatcher().RunAsync(
                winrt::Windows::UI::Core::CoreDispatcherPriority::High,
                UpdateTitlebarColors
            );
        }
    }
}