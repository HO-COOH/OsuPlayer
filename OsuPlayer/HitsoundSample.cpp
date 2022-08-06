#include "pch.h"
#include "HitsoundSample.h"
#if __has_include("HitsoundSample.g.cpp")
#include "HitsoundSample.g.cpp"
#endif

#include "ViewModel.HitsoundSample.g.h"

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::OsuPlayer::implementation
{
    winrt::Windows::UI::Xaml::DependencyProperty HitsoundSample::m_hitsoundNameProperty =
        winrt::Windows::UI::Xaml::DependencyProperty::Register(
            L"HitsoundName",
            winrt::xaml_typename<winrt::Windows::Foundation::IInspectable>(),
            winrt::xaml_typename<OsuPlayer::HitsoundSample>(),
            { nullptr }
    );

    winrt::Windows::UI::Xaml::DependencyProperty HitsoundSample::m_sampleProperty =
        winrt::Windows::UI::Xaml::DependencyProperty::Register(
            L"Sample",
            winrt::xaml_typename<winrt::Windows::Foundation::IInspectable>(),
            winrt::xaml_typename<OsuPlayer::HitsoundSample>(),
            { nullptr }
    );

    HitsoundSample::HitsoundSample()
    {
        InitializeComponent();
    }

    winrt::hstring HitsoundSample::HitsoundName()
    {
        return winrt::unbox_value<winrt::hstring>(GetValue(m_hitsoundNameProperty));
    }

    void HitsoundSample::HitsoundName(winrt::hstring name)
    {
        SetValue(m_hitsoundNameProperty, winrt::box_value(name));
    }

    winrt::Windows::UI::Xaml::DependencyProperty HitsoundSample::HitsoundNameProperty()
    {
        return m_hitsoundNameProperty;
    }

    ViewModel::HitsoundSample HitsoundSample::Sample()
    {
        return winrt::unbox_value<ViewModel::HitsoundSample>(GetValue(m_sampleProperty));
    }

    void HitsoundSample::Sample(ViewModel::HitsoundSample sample)
    {
        sample.Name(HitsoundName());
        return SetValue(m_sampleProperty, winrt::box_value(sample));
    }

    winrt::Windows::UI::Xaml::DependencyProperty HitsoundSample::SampleProperty()
    {
        return m_sampleProperty;
    }
}


void winrt::OsuPlayer::implementation::HitsoundSample::PlayButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{

}


void winrt::OsuPlayer::implementation::HitsoundSample::FileButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{

}
