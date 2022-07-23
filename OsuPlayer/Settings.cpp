#include "pch.h"
#include "Settings.h"
#if __has_include("Settings.g.cpp")
#include "Settings.g.cpp"
#endif
#include "Utils.ThemeHelper.h"
#include "winrt/Windows.Storage.Pickers.h"
#include "ViewModel.SettingsViewModel.g.h"
#include "ViewModelLocator.h"
#include <winrt/Windows.UI.ViewManagement.h>
#include "Model.MyMusic.h"


using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace Utils;

namespace winrt::OsuPlayer::implementation
{
    Settings::Settings()
    {
        InitializeComponent();
    }

    OsuPlayer::ViewModel::SettingsViewModel Settings::ViewModel()
    {
        return ViewModelLocator::Current().SettingsViewModel();
    }


    void Settings::DefaultHitsoundButton_Click(
        winrt::Microsoft::UI::Xaml::Controls::SplitButton const& sender, 
        winrt::Microsoft::UI::Xaml::Controls::SplitButtonClickEventArgs const& args)
    {
    }

    winrt::hstring Settings::DefaultModString()
    {
        switch (ViewModel().DefaultMod())
        {
            case ViewModel::Mod::DoubleTime:    return L"DoubleTime";
            case ViewModel::Mod::HalfTime:      return L"HalfTime";
            case ViewModel::Mod::NightCore:     return L"NightCore";
            case ViewModel::Mod::Normal:        return L"Normal";
        }
    }

}


void winrt::OsuPlayer::implementation::Settings::HalfTimeItem_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
    ViewModel().DefaultMod(ViewModel::Mod::HalfTime);
    raisePropertyChange(L"DefaultModString");
}


void winrt::OsuPlayer::implementation::Settings::DoubleTimeItem_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
    ViewModel().DefaultMod(ViewModel::Mod::DoubleTime);
    raisePropertyChange(L"DefaultModString");
}


void winrt::OsuPlayer::implementation::Settings::NightCoreItem_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
    ViewModel().DefaultMod(ViewModel::Mod::NightCore);
    raisePropertyChange(L"DefaultModString");
}


winrt::Windows::Foundation::IAsyncAction winrt::OsuPlayer::implementation::Settings::NewFolderButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
    while (true)
    {
        auto const result = co_await ViewModel().AddOsuPath();
        if (result == ViewModel::AddOsuFolderResult::Success)
            co_return;

        constexpr auto InvalidPrompt = L"Invalid osu! folder!";
        constexpr auto DuplicatePrompt = L"Duplicate osu! folder!";

        winrt::Windows::UI::Xaml::Controls::ContentDialog invalidOsuFolderDialog{};

        switch (result)
        {
            case ViewModel::AddOsuFolderResult::Duplicate:  invalidOsuFolderDialog.Title(winrt::box_value(DuplicatePrompt));    break;
            default:                                        invalidOsuFolderDialog.Title(winrt::box_value(InvalidPrompt));      break;
        }

        invalidOsuFolderDialog.PrimaryButtonText(L"Retry");
        invalidOsuFolderDialog.CloseButtonText(L"Cancel");
        if (auto result = co_await invalidOsuFolderDialog.ShowAsync(); result != winrt::Windows::UI::Xaml::Controls::ContentDialogResult::Primary)
            co_return;
    }
}
