#pragma once

#include "winrt/Windows.UI.Xaml.h"
#include "winrt/Windows.UI.Xaml.Markup.h"
#include "winrt/Windows.UI.Xaml.Interop.h"
#include "winrt/Windows.UI.Xaml.Controls.Primitives.h"
#include "ColumnHeaderSettingDialog.g.h"
#include "ViewModel.ColumnSettings.g.h"

namespace winrt::OsuPlayer::implementation
{
    struct ColumnHeaderSettingDialog : ColumnHeaderSettingDialogT<ColumnHeaderSettingDialog>
    {
        ColumnHeaderSettingDialog();

        ViewModel::ColumnSettings ViewModel();
    };
}

namespace winrt::OsuPlayer::factory_implementation
{
    struct ColumnHeaderSettingDialog : ColumnHeaderSettingDialogT<ColumnHeaderSettingDialog, implementation::ColumnHeaderSettingDialog>
    {
    };
}
