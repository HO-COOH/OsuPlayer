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

    void Model::Recent::addRecordForSong(SongItemModel const& songItem)
    {
        //the song is in the recent list
        if (auto mapIter = m_recentMap.find(&songItem); mapIter != m_recentMap.cend())
        {
            m_recentList.erase(mapIter->second);
            m_recentList.push_front(&songItem);
            mapIter->second = m_recentList.begin();
        }
        else
        {
            m_recentList.push_front(&songItem);
            m_recentMap[&songItem] = m_recentList.begin();
        }
    }

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
            [&items](SongItemModel const* songItem)
            {
                auto item = winrt::Windows::UI::StartScreen::JumpListItem::CreateWithArguments(
                    L"",
                    songItem->SongName() + L" - " + songItem->Singer()
                );
                item.GroupName(L"Songs");
                items.Append(item);
            }
        );

        //Add collections
        //...
        co_await jumpList.SaveAsync();
    }
}
