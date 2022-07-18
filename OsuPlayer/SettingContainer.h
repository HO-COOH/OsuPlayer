#pragma once

#include "SettingContainer.g.h"

namespace winrt::OsuPlayer::implementation
{
    struct SettingContainer : SettingContainerT<SettingContainer>
    {
        SettingContainer() = default;

        winrt::Windows::Foundation::IInspectable Symbol();
        void Symbol(winrt::Windows::Foundation::IInspectable symbol);
        static winrt::Windows::UI::Xaml::DependencyProperty SymbolProperty();

        winrt::hstring Title();
        void Title(winrt::hstring title);
        static winrt::Windows::UI::Xaml::DependencyProperty TitleProperty();

        winrt::hstring HelpText();
        void HelpText(winrt::hstring helpText);
        static winrt::Windows::UI::Xaml::DependencyProperty HelpTextProperty();

        winrt::Windows::UI::Xaml::Visibility ShowReset();
        void ShowReset(winrt::Windows::UI::Xaml::Visibility showReset);
        static winrt::Windows::UI::Xaml::DependencyProperty ShowResetProperty();

        winrt::Windows::Foundation::IInspectable HeaderContent();
        void HeaderContent(winrt::Windows::Foundation::IInspectable header);
        static winrt::Windows::UI::Xaml::DependencyProperty HeaderContentProperty();

        winrt::Windows::Foundation::IInspectable ExpanderContent();
        void ExpanderContent(winrt::Windows::Foundation::IInspectable expanderContent);
        static winrt::Windows::UI::Xaml::DependencyProperty ExpanderContentProperty();
    private:
        static winrt::Windows::UI::Xaml::DependencyProperty m_symbolProperty;
        static winrt::Windows::UI::Xaml::DependencyProperty m_titleProperty;
        static winrt::Windows::UI::Xaml::DependencyProperty m_helpTextProperty;
        static winrt::Windows::UI::Xaml::DependencyProperty m_showResetProperty;
        static winrt::Windows::UI::Xaml::DependencyProperty m_headerContentProperty;
        static winrt::Windows::UI::Xaml::DependencyProperty m_expanderContentProperty;
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct SettingContainer : SettingContainerT<SettingContainer, implementation::SettingContainer>
    {
    };
}
