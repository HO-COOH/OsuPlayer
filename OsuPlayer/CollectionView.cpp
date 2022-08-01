﻿#include "pch.h"
#include "CollectionView.h"
#if __has_include("CollectionView.g.cpp")
#include "CollectionView.g.cpp"
#endif
#include <winrt/Windows.Foundation.Collections.h>
#include "ViewModelLocator.h"

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::OsuPlayer::implementation
{
    CollectionView::CollectionView()
    {
        InitializeComponent();
    }

    ViewModel::MyMusicViewModel CollectionView::ViewModel()
    {
        return ViewModelLocator::Current().MyMusicViewModel();
    }
}
