#pragma once

#include "Model.SongItem.h"

namespace Model
{
	class Recent
	{
	public:
		static Recent& GetInstance();

		void addRecordForSong(SongItemModel const& songItem);

	};
}
