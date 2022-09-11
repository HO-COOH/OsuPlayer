#include "pch.h"
#include "Settings.h"
#if __has_include("Settings.g.cpp")
#include "Settings.g.cpp"
#endif
#include "Utils.ThemeHelper.h"
#include "winrt/Windows.Storage.Pickers.h"
#include "ViewModel.SettingsViewModel.g.h"
#include "ViewModelLocator.h"
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

    winrt::hstring Settings::DefaultModString()
    {
        switch (ViewModel().DefaultMod())
        {
            case ViewModel::Mod::DoubleTime:    return L"DoubleTime";
            case ViewModel::Mod::HalfTime:      return L"HalfTime";
            case ViewModel::Mod::NightCore:     return L"NightCore";
            case ViewModel::Mod::Normal:        return L"Normal";
        }
        assert(false); //unknown mod?
    }

    void Settings::HalfTimeItem_Click(
        [[maybe_unused]] winrt::Windows::Foundation::IInspectable const& sender,
        [[maybe_unused]] winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        ViewModel().DefaultMod(ViewModel::Mod::HalfTime);
        raisePropertyChange(L"DefaultModString");
    }


    void Settings::DoubleTimeItem_Click(
        [[maybe_unused]] winrt::Windows::Foundation::IInspectable const& sender,
        [[maybe_unused]] winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        ViewModel().DefaultMod(ViewModel::Mod::DoubleTime);
        raisePropertyChange(L"DefaultModString");
    }


    void Settings::NightCoreItem_Click(
        [[maybe_unused]] winrt::Windows::Foundation::IInspectable const& sender,
        [[maybe_unused]] winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        ViewModel().DefaultMod(ViewModel::Mod::NightCore);
        raisePropertyChange(L"DefaultModString");
    }


    winrt::Windows::Foundation::IAsyncAction Settings::NewFolderButton_Click(
        [[maybe_unused]] winrt::Windows::Foundation::IInspectable const& sender,
        [[maybe_unused]] winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        while (true)
        {
            auto const addOsuFolderResult = co_await ViewModel().AddOsuPath();
            if (addOsuFolderResult == ViewModel::AddOsuFolderResult::Success)
            {
                fillSkinMenuFlyout();
                co_return;
            }

            winrt::Windows::UI::Xaml::Controls::ContentDialog invalidOsuFolderDialog{};
            switch (addOsuFolderResult)
            {
                case ViewModel::AddOsuFolderResult::Duplicate:  
                    invalidOsuFolderDialog.Title(winrt::box_value(winrt::Windows::ApplicationModel::Resources::ResourceLoader::GetForCurrentView().GetString(L"DuplicateOsuPathPrompt")));    
                    break;
                default:                                        
                    invalidOsuFolderDialog.Title(winrt::box_value(winrt::Windows::ApplicationModel::Resources::ResourceLoader::GetForCurrentView().GetString(L"InvalidOsuPathPrompt")));
                    break;
            }

            invalidOsuFolderDialog.PrimaryButtonText(winrt::Windows::ApplicationModel::Resources::ResourceLoader::GetForCurrentView().GetString(L"RetryText"));
            invalidOsuFolderDialog.CloseButtonText(winrt::Windows::ApplicationModel::Resources::ResourceLoader::GetForCurrentView().GetString(L"CancelText"));
            if (auto promptResult = co_await invalidOsuFolderDialog.ShowAsync(); promptResult != winrt::Windows::UI::Xaml::Controls::ContentDialogResult::Primary)
                co_return;
        }
    }

    void Settings::fillSkinMenuFlyout()
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
    }

    void Settings::SkinButtonText_Loaded(
        [[maybe_unused]]winrt::Windows::Foundation::IInspectable const& sender,
        [[maybe_unused]]winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        fillSkinMenuFlyout();
        auto const& selectedSkin = ViewModelLocator::Current().SettingsViewModel().SelectedSkin();
        SkinButtonText().Text(selectedSkin ? selectedSkin.Path() : L"Default");
        raisePropertyChange(L"SelectedSkin");
    }
}
