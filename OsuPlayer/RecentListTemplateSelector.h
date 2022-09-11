#pragma once

#include "RecentListTemplateSelector.g.h"

namespace winrt::OsuPlayer::implementation
{
    struct RecentListTemplateSelector : RecentListTemplateSelectorT<RecentListTemplateSelector>
    {
        RecentListTemplateSelector() = default;

        winrt::Windows::UI::Xaml::DataTemplate SelectTemplateCore(winrt::Windows::Foundation::IInspectable const& object);
        winrt::Windows::UI::Xaml::DataTemplate SelectTemplateCore(
            winrt::Windows::Foundation::IInspectable const& object, 
            winrt::Windows::UI::Xaml::DependencyObject container);

        winrt::Windows::UI::Xaml::DataTemplate SongTemplate() { return m_songTemplate; }
        void SongTemplate(winrt::Windows::UI::Xaml::DataTemplate dataTemplate) { m_songTemplate = dataTemplate; }

        winrt::Windows::UI::Xaml::DataTemplate CollectionTemplate() { return m_collectionTemplate; }
        void CollectionTemplate(winrt::Windows::UI::Xaml::DataTemplate dataTemplate) { m_collectionTemplate = dataTemplate; }
    private:
        winrt::Windows::UI::Xaml::DataTemplate m_songTemplate{ nullptr };
        winrt::Windows::UI::Xaml::DataTemplate m_collectionTemplate{ nullptr };
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct RecentListTemplateSelector : RecentListTemplateSelectorT<RecentListTemplateSelector, implementation::RecentListTemplateSelector>
    {
    };
}
