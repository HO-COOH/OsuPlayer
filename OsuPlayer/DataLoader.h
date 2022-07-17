#pragma once

#include <winrt/Windows.Storage.h>
#include <vector>
#include "Model.SongItem.h"

class DataLoader
{
public:
	static std::vector<winrt::Windows::Storage::StorageFolder> LoadOsuFolders();
	static std::vector<Model::SongItemModel> LoadSongs();
private:

};


