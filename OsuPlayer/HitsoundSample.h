#pragma once

#include "winrt/Windows.UI.Xaml.h"
#include "winrt/Windows.UI.Xaml.Markup.h"
#include "winrt/Windows.UI.Xaml.Interop.h"
#include "winrt/Windows.UI.Xaml.Controls.Primitives.h"
#include "HitsoundSample.g.h"

namespace winrt::OsuPlayer::implementation
{
    struct HitsoundSample : HitsoundSampleT<HitsoundSample>
    {
        HitsoundSample();

        ViewModel::HitsoundSample Sample();
        void Sample(ViewModel::HitsoundSample sample);
        static winrt::Windows::UI::Xaml::DependencyProperty SampleProperty();
    private:
        static winrt::Windows::UI::Xaml::DependencyProperty m_sampleProperty;
    public:
        void FileButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct HitsoundSample : HitsoundSampleT<HitsoundSample, implementation::HitsoundSample>
    {
    };
}
