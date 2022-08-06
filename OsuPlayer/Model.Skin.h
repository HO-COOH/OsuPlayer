#pragma once

#include "OsuSkinParser.hpp"
#include <winrt/Windows.Storage.h>
#include <functional>
#include <vector>

namespace Model
{
	struct Skin
	{
		Osu::SkinInfo m_info;
		winrt::Windows::Storage::StorageFolder m_folder;
	public:
		Skin(winrt::Windows::Storage::StorageFolder folder);
		Osu::SkinInfo const& getInfo() const { return m_info; }
	};

	struct Skins
	{
		std::vector<std::function<void()>> m_handlers;
	public:
		std::vector<Skin> m_skins;
		
		static Skins& GetInstance();

		void add(winrt::Windows::Storage::StorageFolder skinFolder);

		template<typename Func>
		void onIndexingFinished(Func&& f)
		{
		}
	};
}