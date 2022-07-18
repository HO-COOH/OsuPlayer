﻿#include "pch.h"
#include "SettingContainer.h"
#if __has_include("SettingContainer.g.cpp")
#include "SettingContainer.g.cpp"
#endif


namespace winrt::OsuPlayer::implementation
{
    winrt::Windows::UI::Xaml::DependencyProperty SettingContainer::m_symbolProperty =
        winrt::Windows::UI::Xaml::DependencyProperty::Register(
            L"Symbol",
            winrt::xaml_typename<winrt::Windows::Foundation::IInspectable>(),
            winrt::xaml_typename<OsuPlayer::SettingContainer>(),
            Windows::UI::Xaml::PropertyMetadata::Create(winrt::Windows::Foundation::IInspectable{})
        );

    winrt::Windows::UI::Xaml::DependencyProperty SettingContainer::m_titleProperty =
        winrt::Windows::UI::Xaml::DependencyProperty::Register(
            L"Title",
            winrt::xaml_typename<winrt::hstring>(),
            winrt::xaml_typename<OsuPlayer::SettingContainer>(),
            { nullptr }
    );

    winrt::Windows::UI::Xaml::DependencyProperty SettingContainer::m_helpTextProperty =
        winrt::Windows::UI::Xaml::DependencyProperty::Register(
            L"HelpText",
            winrt::xaml_typename<winrt::hstring>(),
            winrt::xaml_typename<OsuPlayer::SettingContainer>(),
            { nullptr }
    );

    winrt::Windows::UI::Xaml::DependencyProperty SettingContainer::m_showResetProperty =
        winrt::Windows::UI::Xaml::DependencyProperty::Register(
            L"ShowReset",
            winrt::xaml_typename<winrt::Windows::UI::Xaml::Visibility>(),
            winrt::xaml_typename<OsuPlayer::SettingContainer>(),
            /*Windows::UI::Xaml::PropertyMetadata{ winrt::box_value(false) }*/{ nullptr }
    );

    winrt::Windows::UI::Xaml::DependencyProperty SettingContainer::m_headerContentProperty =
        winrt::Windows::UI::Xaml::DependencyProperty::Register(
            L"HeaderContent",
            winrt::xaml_typename<winrt::Windows::Foundation::IInspectable>(),
            winrt::xaml_typename<OsuPlayer::SettingContainer>(),
            { nullptr }
    );

    winrt::Windows::UI::Xaml::DependencyProperty SettingContainer::m_expanderContentProperty =
        winrt::Windows::UI::Xaml::DependencyProperty::Register(
            L"ExpanderContent",
            winrt::xaml_typename<winrt::Windows::Foundation::IInspectable>(),
            winrt::xaml_typename<OsuPlayer::SettingContainer>(),
            { nullptr }
    );


    winrt::Windows::Foundation::IInspectable implementation::SettingContainer::Symbol()
    {
        return GetValue(m_symbolProperty);
    }

    winrt::Windows::Foundation::IInspectable implementation::SettingContainer::HeaderContent()
    {
        return GetValue(m_headerContentProperty);
    }

    void implementation::SettingContainer::HeaderContent(winrt::Windows::Foundation::IInspectable header)
    {
        SetValue(m_headerContentProperty, header);
    }

    winrt::Windows::Foundation::IInspectable implementation::SettingContainer::ExpanderContent()
    {
        return GetValue(m_expanderContentProperty);
    }

    void implementation::SettingContainer::ExpanderContent(winrt::Windows::Foundation::IInspectable expanderContent)
    {
        SetValue(m_expanderContentProperty, expanderContent);
    }

    void implementation::SettingContainer::Symbol(winrt::Windows::Foundation::IInspectable symbol)
    {
        SetValue(m_symbolProperty, symbol);
    }

    winrt::Windows::UI::Xaml::DependencyProperty implementation::SettingContainer::ExpanderContentProperty()
    {
        return m_expanderContentProperty;
    }

    winrt::Windows::UI::Xaml::DependencyProperty implementation::SettingContainer::HeaderContentProperty()
    {
        return m_headerContentProperty;
    }

    winrt::Windows::UI::Xaml::DependencyProperty implementation::SettingContainer::SymbolProperty()
    {
        return m_symbolProperty;
    }

    winrt::hstring implementation::SettingContainer::Title()
    {
        return winrt::unbox_value<winrt::hstring>(GetValue(m_titleProperty));
    }
    void implementation::SettingContainer::Title(winrt::hstring title)
    {
        SetValue(m_titleProperty, winrt::box_value(title));
    }
    winrt::hstring implementation::SettingContainer::HelpText()
    {
        return winrt::unbox_value<winrt::hstring>(GetValue(m_helpTextProperty));
    }
    void implementation::SettingContainer::HelpText(winrt::hstring helpText)
    {
        SetValue(m_helpTextProperty, winrt::box_value(helpText));
    }
    winrt::Windows::UI::Xaml::Visibility implementation::SettingContainer::ShowReset()
    {
        return winrt::unbox_value<winrt::Windows::UI::Xaml::Visibility>(GetValue(m_showResetProperty));
    }
    void implementation::SettingContainer::ShowReset(winrt::Windows::UI::Xaml::Visibility showReset)
    {
        SetValue(m_showResetProperty, winrt::box_value(showReset));
    }
    winrt::Windows::UI::Xaml::DependencyProperty implementation::SettingContainer::ShowResetProperty()
    {
        return m_showResetProperty;
    }

    winrt::Windows::UI::Xaml::DependencyProperty implementation::SettingContainer::TitleProperty()
    {
        return m_titleProperty;
    }
    winrt::Windows::UI::Xaml::DependencyProperty implementation::SettingContainer::HelpTextProperty()
    {
        return m_helpTextProperty;
    }
}
