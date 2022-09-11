#include "pch.h"
#include "ViewModelLocator.h"

ViewModelLocator& ViewModelLocator::Current()
{
    static ViewModelLocator locator;
    return locator;
}

winrt::OsuPlayer::ViewModel::PlayerViewModel ViewModelLocator::PlayerViewModel()
{
    if (m_playerViewModel == nullptr)
        m_playerViewModel = winrt::OsuPlayer::ViewModel::PlayerViewModel{};
    return m_playerViewModel;
}

winrt::OsuPlayer::ViewModel::MyMusicViewModel ViewModelLocator::MyMusicViewModel()
{
    if (m_myMusicViewModel == nullptr)
        m_myMusicViewModel = winrt::OsuPlayer::ViewModel::MyMusicViewModel{};
    return m_myMusicViewModel;
}

winrt::OsuPlayer::ViewModel::SettingsViewModel ViewModelLocator::SettingsViewModel()
{
    if (m_settingsViewModel == nullptr)
        m_settingsViewModel = winrt::OsuPlayer::ViewModel::SettingsViewModel{};
    return m_settingsViewModel;
}

winrt::OsuPlayer::ViewModel::ColumnSettings ViewModelLocator::ColumnSettings()
{
    if (m_columnSettingsViewModel == nullptr)
        m_columnSettingsViewModel = winrt::OsuPlayer::ViewModel::ColumnSettings{};
    return m_columnSettingsViewModel;
}

winrt::OsuPlayer::ViewModel::HitsoundPanelViewModel ViewModelLocator::HitsoundPanelViewModel()
{
    static winrt::OsuPlayer::ViewModel::HitsoundPanelViewModel model;
    return model;
}

winrt::OsuPlayer::ViewModel::RecentViewModel ViewModelLocator::RecentViewModel()
{
    if (m_recentViewModel == nullptr)
        m_recentViewModel = winrt::OsuPlayer::ViewModel::RecentViewModel{};
    return m_recentViewModel;
}

winrt::OsuPlayer::ViewModel::MyMusicViewModel ViewModelLocator::getCollectionPageByCollectionItem(winrt::OsuPlayer::ViewModel::CollectionItem collectionItem)
{
    return getCollectionPageByName(collectionItem.Name());
}

winrt::OsuPlayer::ViewModel::MyMusicViewModel ViewModelLocator::getCollectionPageByName(winrt::hstring collectionName)
{
    //there is already a collection page view model
    if (auto iter = CollectionsPageMap.find(collectionName); iter != CollectionsPageMap.cend())
        return CollectionsPageMap[collectionName];
    
    //construct a collection page view model from a colleciton view model
    if (auto iter = CollectionsNameMap.find(collectionName); iter != CollectionsNameMap.cend())
    {
        winrt::OsuPlayer::ViewModel::MyMusicViewModel viewModel;
        viewModel.ListName(iter->second.Name());
        viewModel.Songs(iter->second.SongItems());
        CollectionsPageMap[collectionName] = viewModel;
        return viewModel;
    }
    
    //No such collection exist
    assert(false);
}

void ViewModelLocator::updateCollectionItem(winrt::OsuPlayer::ViewModel::CollectionItem& item)
{
    CollectionsNameMap[item.Name()] = item;
}
