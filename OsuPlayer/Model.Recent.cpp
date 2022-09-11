#include "pch.h"
#include "Model.Recent.h"
#include "ViewModelLocator.h"
#include <winrt/Windows.UI.StartScreen.h>
namespace Model
{
    Recent& Recent::GetInstance()
    {
        static Recent s_recent;
        return s_recent;
    }

    template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
    template<class... Ts> overloaded(Ts...)->overloaded<Ts...>;

    winrt::fire_and_forget Recent::updateJumpList()
    {
        auto const numJumpListSongs = ViewModelLocator::Current().SettingsViewModel().JumpListRecentSongs();
        auto jumpList = co_await winrt::Windows::UI::StartScreen::JumpList::LoadCurrentAsync();
        jumpList.SystemGroupKind(winrt::Windows::UI::StartScreen::JumpListSystemGroupKind::Recent);
        auto items = jumpList.Items();
        items.Clear();
        std::for_each_n(
            m_recentList.cbegin(),
            min(m_recentList.size(), numJumpListSongs),
            [&items](auto const& songOrCollection)
            {
                std::visit(
                    overloaded
                    {
                        [&items](SongItemModel const* song) 
                        {
                            auto item = winrt::Windows::UI::StartScreen::JumpListItem::CreateWithArguments(
                                 L"",
                                 song->SongName() + L" - " + song->Singer()
                            );
                            item.GroupName(L"Songs");
                            items.Append(item);
                        },
                        [&items](CollectionItemModel const* collection)
                        {
                            auto item = winrt::Windows::UI::StartScreen::JumpListItem::CreateWithArguments(
                                L"",
                                winrt::to_hstring(collection->m_name)
                            );
                            item.GroupName(L"Collections");
                            items.Append(item);
                        }
                    },
                    songOrCollection
                );
            }
        );

        co_await jumpList.SaveAsync();
    }
}
