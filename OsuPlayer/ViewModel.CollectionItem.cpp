#include "pch.h"
#include "ViewModel.CollectionItem.h"
#if __has_include("ViewModel.CollectionItem.g.cpp")
#include "ViewModel.CollectionItem.g.cpp"
#endif

namespace winrt::OsuPlayer::ViewModel::implementation
{
	winrt::hstring CollectionItem::Name()
	{
		return winrt::to_hstring(m_modelPointer->m_name);
	}

	Windows::Foundation::Collections::IObservableVector<ViewModel::SongItemViewModel> CollectionItem::SongItems()
	{
		return m_songItems;
	}
	winrt::Windows::Foundation::IInspectable CollectionItem::ModelPointer()
	{
		return winrt::box_value<size_t>(reinterpret_cast<size_t>(m_modelPointer));
	}

	void CollectionItem::ModelPointer(winrt::Windows::Foundation::IInspectable modelPointer)
	{
		m_modelPointer = reinterpret_cast<Model::CollectionItemModel*>(winrt::unbox_value<size_t>(modelPointer));
		for (auto songItem : m_modelPointer->m_songItemPtr)
		{
			SongItemViewModel viewModel;
			auto versions = viewModel.Versions();
			for (auto const& difficulty : songItem->Difficulties())
			{
				versions.Append(difficulty);
			}
			viewModel.ModelPointer(winrt::box_value<size_t>(reinterpret_cast<size_t>(songItem)));
			m_songItems.Append(viewModel);
		}
	}

	int CollectionItem::Count()
	{
		return m_songItems.Size();
	}
}
