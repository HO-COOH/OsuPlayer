#include "pch.h"
#include "Settings.h"
#if __has_include("Settings.g.cpp")
#include "Settings.g.cpp"
#endif
#include "Utils.ThemeHelper.h"
#include "winrt/Windows.Storage.Pickers.h"
#include "ViewModel.SettingsViewModel.g.h"
#include "Model.Settings.h"
#include "winrt/Windows.UI.ViewManagement.h"


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
        return m_model;
    }

    void Settings::LightButton_Checked(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        ThemeHelper::RootTheme(winrt::Windows::UI::Xaml::ElementTheme::Light);
    }


    void Settings::DarkButton_Checked(
        winrt::Windows::Foundation::IInspectable const& sender, 
        winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        ThemeHelper::RootTheme(winrt::Windows::UI::Xaml::ElementTheme::Dark);
    }


    void Settings::SystemThemeButton_Checked(
        winrt::Windows::Foundation::IInspectable const& sender, 
        winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        ThemeHelper::RootTheme(winrt::Windows::UI::Xaml::ElementTheme::Default);
    }

    void Settings::ListBox_SelectionChanged(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Windows::UI::Xaml::Controls::SelectionChangedEventArgs const& e)
    {

    }


    winrt::Windows::Foundation::IAsyncAction Settings::AddOsuPathButton_Click(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        bool hasException{};
        do
        {
            try
            {
                co_await m_model.AddOsuPath();
            }
            catch (Model::InvalidOsuFolderException const& e)
            {
                hasException = true;
            }
            if (hasException)
            {
                winrt::Windows::UI::Xaml::Controls::ContentDialog invalidOsuFolderDialog{};
                invalidOsuFolderDialog.Title(winrt::box_value(L"Invalid osu! folder!"));
                invalidOsuFolderDialog.PrimaryButtonText(L"Retry");
                invalidOsuFolderDialog.CloseButtonText(L"Cancel");
                if (auto result = co_await invalidOsuFolderDialog.ShowAsync(); result != winrt::Windows::UI::Xaml::Controls::ContentDialogResult::Primary)
                    hasException = false;
            }
        } while (hasException);
    }


    void Settings::ClearAllButton_Click(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        m_model.ClearAll();
    }


    void Settings::DefaultHitsoundButton_Click(
        winrt::Microsoft::UI::Xaml::Controls::SplitButton const& sender, 
        winrt::Microsoft::UI::Xaml::Controls::SplitButtonClickEventArgs const& args)
    {
    }


    void Settings::HalfTimeButton_Checked(
        winrt::Windows::Foundation::IInspectable const& sender, 
        winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        m_model.DefaultMod(ViewModel::Mod::HalfTime);
    }


    void Settings::DoubleTimeButton_Checked(
        winrt::Windows::Foundation::IInspectable const& sender, 
        winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        m_model.DefaultMod(ViewModel::Mod::DoubleTime);
    }


    void Settings::NightCoreButton_Checked(
        winrt::Windows::Foundation::IInspectable const& sender, 
        winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        m_model.DefaultMod(ViewModel::Mod::NightCore);
    }


    void Settings::HalfTimeButton_Unchecked(
        winrt::Windows::Foundation::IInspectable const& sender, 
        winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        m_model.DefaultMod(ViewModel::Mod::Normal);
    }


    void Settings::DoubleTimeButton_Unchecked(
        winrt::Windows::Foundation::IInspectable const& sender, 
        winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        m_model.DefaultMod(ViewModel::Mod::Normal);
    }


    void Settings::NightCoreButton_Unchecked(
        winrt::Windows::Foundation::IInspectable const& sender, 
        winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        m_model.DefaultMod(ViewModel::Mod::Normal);
    }

    winrt::Windows::Foundation::IAsyncAction Settings::AllowModifyOsuDataCheckbox_Checked(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        auto result = co_await ExperimentFeatureWarningDialog().ShowAsync();
        if (result != winrt::Windows::UI::Xaml::Controls::ContentDialogResult::Secondary)
        {
            //Do not allow modifying osu! data
            //AllowModifyOsuDataCheckbox().IsChecked(false);
        }
        else
        {
            //Allow modifying osu! data

        }
    }

}
