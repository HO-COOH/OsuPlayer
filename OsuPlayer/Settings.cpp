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
#include "Model.Skin.h"
#include "HitsoundPanel.g.h"
#include <algorithm>


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
        {
            auto skinFlyoutItems = SkinFlyout().Items();
            skinFlyoutItems.Clear();
            for (auto const& skin : Model::Skins::GetInstance().m_skins)
            {
                winrt::Windows::UI::Xaml::Controls::MenuFlyoutItem item;
                item.Text(winrt::to_hstring(skin.getInfo().name));
                item.Click([this](auto sender, auto e) -> winrt::Windows::Foundation::IAsyncAction
                {
                    auto& allSkins = Model::Skins::GetInstance().m_skins;
                    auto text = sender.as<winrt::Windows::UI::Xaml::Controls::MenuFlyoutItem>().Text();
                    auto skinName = winrt::to_string(text);
                    SkinButtonText().Text(text);
                    auto item = std::find_if(allSkins.begin(), allSkins.end(), [skinName](Model::Skin const& skinItem) { return skinItem.getInfo().name == skinName; });
                    auto& viewModelLoctor = ViewModelLocator::Current();
                    viewModelLoctor.SettingsViewModel().SelectedSkin(item->m_folder);
                    co_await viewModelLoctor.HitsoundPanelViewModel().Update();
                });
                skinFlyoutItems.Append(item);
            }
            co_return;
        }

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
