#pragma once

#include <winrt/Windows.Storage.h>
#include <vector>
#include "SongItemModel.h"

class DataLoader
{
public:
	static std::vector<winrt::Windows::Storage::StorageFolder> LoadOsuFolders();
	static std::vector<SongItemModel> LoadSongs();
private:

};


