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
#include "ViewModelLocator.h"
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.ApplicationModel.DataTransfer.h>

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

static auto GetSearchPrefix(bool isOsuSearch = true)
{
    constexpr auto OsuSearchPrefix = L"https://osu.ppy.sh/beatmapsets?q=";
    return isOsuSearch ? winrt::hstring{ OsuSearchPrefix } : ViewModelLocator::Current().SettingsViewModel().CustomSearchPrefix();
}

static auto MakeSearchLink(std::wstring_view text, bool isOsuSearch = true)
{
    return winrt::Windows::Foundation::Uri(GetSearchPrefix(isOsuSearch) + text);
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
        m_title = title;
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

 
    winrt::Windows::Foundation::IAsyncAction SongItemDialog::CopyOnLinkClick(winrt::Windows::UI::Xaml::Documents::Hyperlink link, winrt::Windows::UI::Xaml::Documents::HyperlinkClickEventArgs args)
    {
        winrt::Windows::ApplicationModel::DataTransfer::DataPackage package;
        package.SetText(link.Inlines().GetAt(0).as<winrt::Windows::UI::Xaml::Documents::Run>().Text());
        winrt::Windows::ApplicationModel::DataTransfer::Clipboard::SetContent(package);

        Tip().IsOpen(true);
        co_await std::chrono::seconds{ 1 };
        co_await winrt::resume_foreground(this->Dispatcher());
        Tip().IsOpen(false);
    }

    
    winrt::Windows::UI::Xaml::Documents::Paragraph SongItemDialog::HandleTagsRichText(winrt::hstring const& str)
    {
        auto const linkAction = ViewModelLocator::Current().SettingsViewModel().LinkActionIndex(); //don't check it in the for-loop
        winrt::Windows::UI::Xaml::Documents::Paragraph paragraph;
        for (auto tag : Split(std::wstring_view{ str }, L' '))
        {
            if (tag == L"")
                continue;   //otherwise exception on constructing empty uri

            winrt::Windows::UI::Xaml::Documents::Hyperlink link;
            winrt::Windows::UI::Xaml::Documents::Run run[2];
            run[0].Text(tag);   //This is for showing the link's text
            run[1].Text(L" ");  //This is for adding a space separating the links

            switch (linkAction)
            {
                case 1: link.Click({ this, &SongItemDialog::CopyOnLinkClick }); break;
                case 2: link.NavigateUri(MakeSearchLink(tag, true)); break;
                case 3: link.NavigateUri(MakeSearchLink(tag, false)); break;
                default: break;
            }
            
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

        try
        {
            switch (ViewModelLocator::Current().SettingsViewModel().LinkActionIndex())
            {
                case 1: link.Click({ this, &SongItemDialog::CopyOnLinkClick }); break;
                case 2: link.NavigateUri(MakeSearchLink(title, true)); break;
                case 3: link.NavigateUri(MakeSearchLink(title, false)); break;
                default: break;
            }
        }
        catch (winrt::hresult_invalid_argument const& e)
        {
            //Custom search link may be invalid, causing an exception in the 2 and 3 case
        }
        link.Inlines().Append(run);
        
        winrt::Windows::UI::Xaml::Documents::Paragraph paragraph;
        paragraph.Inlines().Append(link);

        return paragraph;
    }

    winrt::Windows::Foundation::IAsyncAction SongItemDialog::SongPathButton_Click(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        switch (ViewModelLocator::Current().SettingsViewModel().OsuPathActionIndex())
        {
            case 0: //Open the file
            {
                auto file = co_await winrt::Windows::Storage::StorageFile::GetFileFromPathAsync(SongPathText().Text());
                co_await winrt::Windows::System::Launcher::LaunchFileAsync(file);
                break;
            }
            case 1: //Open folder
            {
                auto file = co_await winrt::Windows::Storage::StorageFile::GetFileFromPathAsync(SongPathText().Text());
                auto folder = co_await file.GetParentAsync();
                co_await winrt::Windows::System::Launcher::LaunchFolderAsync(folder);
                break;
            }
            case 2: //Open osu! and copy the song name
            {
                winrt::Windows::ApplicationModel::DataTransfer::DataPackage package;
                package.SetText(m_title);
                winrt::Windows::ApplicationModel::DataTransfer::Clipboard::SetContent(package);

                /*
                    To launch a process the .exe must either be contained in the .appx package or be registered in the whitelist for this API. 
                    To add an .exe to the whitelist modify HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\EmbeddedMode\ProcessLauncher:AllowedExecutableFilesList 
                    and add your executable to the REG_MULTI_SZ formatted string.

                    co_await winrt::Windows::System::ProcessLauncher::RunToCompletionAsync( L"D:\\osu\\osu!.exe", L"");
                */
                Tip().IsOpen(true);
                co_await std::chrono::seconds{ 1 };
                co_await winrt::resume_foreground(this->Dispatcher());
                Tip().IsOpen(false);
            }
        }
    }
}
