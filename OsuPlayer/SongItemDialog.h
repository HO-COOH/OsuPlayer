#pragma once
#include "SongPropertyViewModel.g.h"
#include "SongItemDialog.g.h"

namespace winrt::OsuPlayer::implementation
{
    struct SongItemDialog : SongItemDialogT<SongItemDialog>
    {
        SongItemDialog();

        OsuPlayer::SongPropertyViewModel ViewModel();

        void ViewModel(OsuPlayer::SongPropertyViewModel viewModel);
    private:

        static winrt::Windows::UI::Xaml::Documents::Paragraph HandleTagsRichText(winrt::hstring const& tags);
        static winrt::Windows::UI::Xaml::Documents::Paragraph HandleTitleRichText(winrt::hstring const& title);
        OsuPlayer::SongPropertyViewModel m_model;
    public:
        winrt::Windows::Foundation::IAsyncAction SongPathButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct SongItemDialog : SongItemDialogT<SongItemDialog, implementation::SongItemDialog>
    {
    };
}
