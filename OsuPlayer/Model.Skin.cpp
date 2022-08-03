#include "pch.h"
#include "Model.Skin.h"
#include "Utils.Log.hpp"

namespace Model
{
	Skin::Skin(winrt::Windows::Storage::StorageFolder folder) : 
		m_info{ folder.GetFileAsync(L"skin.ini").get()}, 
		m_folder{folder}
	{
		Utils::ConsoleLogger{} << m_info.name.data() << L'\n';
	}

	Skins& Skins::GetInstance()
	{
		static Skins s_skins;
		return s_skins;
	}

	void Skins::add(winrt::Windows::Storage::StorageFolder skinFolder)
	{
		for (auto subFolder : skinFolder.GetFoldersAsync().get())
			m_skins.emplace_back(subFolder);

		for (auto& handler : m_handlers)
			handler();
	}
}
