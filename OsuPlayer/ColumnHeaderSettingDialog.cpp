#include "pch.h"
#include "ColumnHeaderSettingDialog.h"
#if __has_include("ColumnHeaderSettingDialog.g.cpp")
#include "ColumnHeaderSettingDialog.g.cpp"
#endif

#include "ViewModelLocator.h"

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::OsuPlayer::implementation
{
    ColumnHeaderSettingDialog::ColumnHeaderSettingDialog()
    {
        InitializeComponent();
    }

    ViewModel::ColumnSettings ColumnHeaderSettingDialog::ViewModel()
    {
        return ViewModelLocator::Current().ColumnSettings();
    }

}
