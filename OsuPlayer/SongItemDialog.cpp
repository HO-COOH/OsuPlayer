#include "pch.h"
#include "SongItemDialog.h"
#if __has_include("SongItemDialog.g.cpp")
#include "SongItemDialog.g.cpp"
#endif

using namespace winrt;
using namespace Windows::UI::Xaml;

#include <winrt/Windows.UI.Xaml.Documents.h>
#include "OsuParser.hpp"
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.System.h>

template<typename Char>
std::vector<std::basic_string<Char>> Split(const std::basic_string_view<Char> str, const Char delim)
{
    std::vector<std::basic_string<Char>> result;

    int indexCommaToLeftOfColumn = 0;
    int indexCommaToRightOfColumn = -1;

    for (int i = 0; i < static_cast<int>(str.size()); i++)
    {
        if (str[i] == delim)
        {
            indexCommaToLeftOfColumn = indexCommaToRightOfColumn;
            indexCommaToRightOfColumn = i;
            int index = indexCommaToLeftOfColumn + 1;
            int length = indexCommaToRightOfColumn - index;

            std::basic_string<Char> column(str.data() + index, length);
            result.push_back(column);
        }
    }
    std::basic_string<Char> finalColumn(str.data() + indexCommaToRightOfColumn + 1, str.size() - indexCommaToRightOfColumn - 1);
    result.push_back(finalColumn);
    return result;
}

namespace winrt::OsuPlayer::implementation
{
    SongItemDialog::SongItemDialog()
    {
        InitializeComponent();
    }


    void SongItemDialog::Title(winrt::hstring title)
    {
        TitleText().Blocks().Append(HandleTitleRichText(title));
    }

    void SongItemDialog::Singer(winrt::hstring singer)
    {
        ArtistText().Blocks().Append(HandleTagsRichText(singer));
    }

    void SongItemDialog::Tags(winrt::hstring tags)
    {
        TagsText().Blocks().Append(HandleTagsRichText(tags));
    }

    void SongItemDialog::Bitrate(winrt::hstring bitrate)
    {
        BitrateText().Text(bitrate);
    }


    void SongItemDialog::Length(winrt::hstring length)
    {
        LengthText().Text(length);
    }

    void SongItemDialog::SongPath(winrt::hstring songPath)
    {
        SongPathText().Text(songPath);
    }


    constexpr auto SearchPrefix = L"https://osu.ppy.sh/beatmapsets?q=";
    winrt::Windows::UI::Xaml::Documents::Paragraph SongItemDialog::HandleTagsRichText(winrt::hstring const& str)
    {
        winrt::Windows::UI::Xaml::Documents::Paragraph paragraph;
        
        for (auto tag : Split(std::wstring_view{ str }, L' '))
        {
            winrt::Windows::UI::Xaml::Documents::Hyperlink link;
            winrt::Windows::UI::Xaml::Documents::Run run[2];
            run[0].Text(tag);   //This is for showing the link's text
            run[1].Text(L" ");  //This is for adding a space separating the links

            link.NavigateUri(winrt::Windows::Foundation::Uri{ winrt::hstring{ SearchPrefix } + tag });
            link.Inlines().Append(run[0]);
            
            paragraph.Inlines().Append(link);
            paragraph.Inlines().Append(run[1]);
        }

        return paragraph;
    }

    winrt::Windows::UI::Xaml::Documents::Paragraph SongItemDialog::HandleTitleRichText(winrt::hstring const& title)
    {
        winrt::Windows::UI::Xaml::Documents::Run run;
        run.Text(title);
        
        winrt::Windows::UI::Xaml::Documents::Hyperlink link;
        link.NavigateUri(winrt::Windows::Foundation::Uri{ winrt::hstring{SearchPrefix} + title });
        link.Inlines().Append(run);
        
        winrt::Windows::UI::Xaml::Documents::Paragraph paragraph;
        paragraph.Inlines().Append(link);

        return paragraph;
    }

    winrt::Windows::Foundation::IAsyncAction SongItemDialog::SongPathButton_Click(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        auto file = co_await winrt::Windows::Storage::StorageFile::GetFileFromPathAsync(SongPathText().Text());
        auto folder = co_await file.GetParentAsync();
        co_await winrt::Windows::System::Launcher::LaunchFolderAsync(folder);
    }

}
