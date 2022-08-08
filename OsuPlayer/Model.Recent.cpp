#include "pch.h"
#include "Model.Recent.h"

namespace Model
{
    Recent& Recent::GetInstance()
    {
        static Recent s_recent;
        return s_recent;
    }

    void Model::Recent::addRecordForSong(SongItemModel const& songItem)
    {
    }
}
