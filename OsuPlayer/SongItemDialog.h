#pragma once
#include "SongPropertyViewModel.g.h"
#include "SongItemDialog.g.h"

namespace winrt::OsuPlayer::implementation
{
    struct SongItemDialog : SongItemDialogT<SongItemDialog>
    {
        SongItemDialog();

        OsuPlayer::SongPropertyViewModel ViewModel();
    private:

        OsuPlayer::SongPropertyViewModel m_model;
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct SongItemDialog : SongItemDialogT<SongItemDialog, implementation::SongItemDialog>
    {
    };
}
