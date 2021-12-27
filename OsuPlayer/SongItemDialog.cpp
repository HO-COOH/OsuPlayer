#include "pch.h"
#include "SongItemDialog.h"
#if __has_include("SongItemDialog.g.cpp")
#include "SongItemDialog.g.cpp"
#endif

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::OsuPlayer::implementation
{
    SongItemDialog::SongItemDialog()
    {
        InitializeComponent();
    }

    OsuPlayer::SongPropertyViewModel SongItemDialog::ViewModel()
    {
        return m_model;
    }
}
