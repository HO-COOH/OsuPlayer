#include "pch.h"
#include "ViewModel.CollectionItem.h"
#if __has_include("ViewModel.CollectionItem.g.cpp")
#include "ViewModel.CollectionItem.g.cpp"
#endif

namespace winrt::OsuPlayer::ViewModel::implementation
{
	winrt::hstring CollectionItem::Name()
	{
		return m_name;
	}
	void CollectionItem::Name(winrt::hstring name)
	{
		m_name = name;
	}
	Windows::Foundation::Collections::IObservableVector<SongItem> CollectionItem::SongItems()
	{
		return m_songItems;
	}
	int CollectionItem::Count()
	{
		return m_songItems.Size();
	}
}
