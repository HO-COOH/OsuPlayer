﻿#include "pch.h"
#include "ViewModel.SongItem.h"
#if __has_include("ViewModel.SongItemViewModel.g.cpp")
#include "ViewModel.SongItemViewModel.g.cpp"
#endif
#include "Utils.h"
#include "OsuParser.hpp"
#include "MyMusic.g.h"
#include "ViewModelLocator.h"
#include <ppltasks.h>
#include <pplawait.h>

namespace winrt::OsuPlayer::ViewModel::implementation
{
    winrt::hstring SongItemViewModel::SongName()
    {
        return getModel().SongName();
    }

    winrt::hstring SongItemViewModel::Singer()
    {
        return getModel().Singer();
    }

    winrt::hstring SongItemViewModel::Mapper()
    {
        return getModel().Mapper();
    }

    winrt::hstring SongItemViewModel::LengthString()
    {
        return Utils::GetDurationString(Utils::GetDuration(Length()));
    }

    int SongItemViewModel::Length()
    {
        return getModel().Length();
    }

    int SongItemViewModel::SelectedVersionIndex()
    {
        return m_versionIndex;
    }

    void SongItemViewModel::SelectedVersionIndex(int index)
    {
        //https://github.com/microsoft/microsoft-ui-xaml/issues/7521
        m_versionIndex = std::clamp(index, 0, static_cast<int>(getModel().m_beatmaps.size() - 1));
    }

    winrt::Windows::Foundation::Collections::IObservableVector<winrt::hstring> SongItemViewModel::Versions()
    {
        return m_versions;
    }


    //winrt::Windows::UI::Xaml::Media::Imaging::BitmapImage SongItemViewModel::Image()
    //{
    //    return winrt::Windows::UI::Xaml::Media::Imaging::BitmapImage();
    //}
    void SongItemViewModel::PlayCurrent()
    {
        ViewModelLocator::Current().PlayerViewModel().Play(*this);
    }
    winrt::Windows::Foundation::IAsyncAction implementation::SongItemViewModel::ShowProperty()
    {
        auto& songItem = getModel();

        if(!songItem.isDataFilled())
            co_await songItem.fillDataAsync();

        int versionIndex = SelectedVersionIndex();


        OsuPlayer::SongItemDialog content;

        auto tagTask = concurrency::create_task([&songItem, versionIndex] {return songItem.Tags(versionIndex); });
        auto bitrateTask = concurrency::create_task([&songItem] {return songItem.BitRate(); });

        content.Tags(winrt::to_hstring(co_await tagTask));
        content.Bitrate(winrt::to_hstring(co_await bitrateTask) + L" kbps");
        content.SongPath(songItem.Difficulties()[versionIndex]);
        content.Title(songItem.SongName());
        content.Singer(songItem.Singer());
        content.Length(Utils::GetDurationString(Utils::GetDuration(songItem.Length())));


        winrt::Windows::UI::Xaml::Controls::ContentDialog propertyDialog;
        propertyDialog.Content(content);
        propertyDialog.CloseButtonText(L"Close");

        co_await propertyDialog.ShowAsync();
    }

    Model::SongItemModel& implementation::SongItemViewModel::getModel()
    {
        return *reinterpret_cast<Model::SongItemModel*>(winrt::unbox_value<size_t>(ModelPointer()));
    }
}
