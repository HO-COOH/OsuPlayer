#include "pch.h"
#include "HitsoundSample.h"
#if __has_include("HitsoundSample.g.cpp")
#include "HitsoundSample.g.cpp"
#endif

#include "ViewModel.HitsoundSample.g.h"
#include <winrt/Windows.System.h>
#include <winrt/Windows.Storage.h>
#include "Utils.h"

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::OsuPlayer::implementation
{
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

    ViewModel::HitsoundSample HitsoundSample::Sample()
    {
        return winrt::unbox_value<ViewModel::HitsoundSample>(GetValue(m_sampleProperty));
    }

    void HitsoundSample::Sample(ViewModel::HitsoundSample sample)
    {
        return SetValue(m_sampleProperty, winrt::box_value(sample));
    }

    winrt::Windows::UI::Xaml::DependencyProperty HitsoundSample::SampleProperty()
    {
        return m_sampleProperty;
    }

    winrt::Windows::Foundation::IAsyncAction HitsoundSample::HyperlinkButton_Click(
        [[maybe_unused]]winrt::Windows::Foundation::IInspectable const& sender, 
        [[maybe_unused]]winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        auto file = Sample().File();
        if (auto folder = co_await file.GetParentAsync(); folder)
            co_await winrt::Windows::System::Launcher::LaunchFolderAsync(folder);
        else
        {
            winrt::Microsoft::UI::Xaml::Controls::TeachingTip tip;
            tip.Title(L"Title");
            //auto tip = Application::Current().Resources().Lookup(winrt::box_value(L"tip")).as<winrt::Microsoft::UI::Xaml::Controls::TeachingTip>();
            tip.Subtitle(Utils::GetFileOpenFailedFormatString(file.Path()));
            tip.ShouldConstrainToRootBounds(true);
            tip.IsOpen(true);

            //TODO: here
        }
    }

}
