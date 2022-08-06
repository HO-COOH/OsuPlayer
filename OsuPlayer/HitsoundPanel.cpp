#include "pch.h"
#include "HitsoundPanel.h"
#if __has_include("HitsoundPanel.g.cpp")
#include "HitsoundPanel.g.cpp"
#endif

#include "ViewModelLocator.h"

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::OsuPlayer::implementation
{
    HitsoundPanel::HitsoundPanel()
    {
        InitializeComponent();
    }
    ViewModel::HitsoundPanelViewModel HitsoundPanel::ViewModel()
    {
        return ViewModelLocator::Current().HitsoundPanelViewModel();
    }
}
