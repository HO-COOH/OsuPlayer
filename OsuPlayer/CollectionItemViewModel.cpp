#include "pch.h"
#include "CollectionItemViewModel.h"
#if __has_include("CollectionItemViewModel.g.cpp")
#include "CollectionItemViewModel.g.cpp"
#endif

namespace winrt::OsuPlayer::implementation
{
	winrt::hstring CollectionItemViewModel::Name()
	{
		return m_name;
	}
	void CollectionItemViewModel::Name(winrt::hstring name)
	{
		m_name = name;
	}
	Windows::Foundation::Collections::IObservableVector<SongItem> CollectionItemViewModel::SongItems()
	{
		return m_songItems;
	}
}
