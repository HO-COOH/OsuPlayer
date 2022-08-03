#pragma once
#include "SongItemDialog.g.h"

namespace winrt::OsuPlayer::implementation
{
    struct SongItemDialog : SongItemDialogT<SongItemDialog>
    {
        SongItemDialog();

        winrt::hstring Title() { return {}; }
        void Title(winrt::hstring title);

        winrt::hstring Singer() { return {}; }
        void Singer(winrt::hstring singer);

        winrt::hstring Tags() { return {}; }
        void Tags(winrt::hstring tags);

        winrt::hstring Bitrate() { return {}; }
        void Bitrate(winrt::hstring bitrate);

        winrt::hstring Length() { return {}; }
        void Length(winrt::hstring length);

        winrt::hstring SongPath() { return {}; }
        void SongPath(winrt::hstring songPath);
    public:
        winrt::hstring m_title;
        winrt::Windows::Foundation::IAsyncAction SongPathButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        winrt::Windows::UI::Xaml::Documents::Paragraph HandleTagsRichText(winrt::hstring const& tags);
        winrt::Windows::UI::Xaml::Documents::Paragraph HandleTitleRichText(winrt::hstring const& title);
        winrt::Windows::Foundation::IAsyncAction CopyOnLinkClick(winrt::Windows::UI::Xaml::Documents::Hyperlink link, winrt::Windows::UI::Xaml::Documents::HyperlinkClickEventArgs args);
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct SongItemDialog : SongItemDialogT<SongItemDialog, implementation::SongItemDialog>
    {
    };
}
