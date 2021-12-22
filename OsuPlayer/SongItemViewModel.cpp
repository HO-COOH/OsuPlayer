#include "pch.h"
#include "SongItemViewModel.h"
#if __has_include("SongItemViewModel.g.cpp")
#include "SongItemViewModel.g.cpp"
#endif
#include "Utils.h"
#include "OsuParser.hpp"

namespace winrt::OsuPlayer::implementation
{
    winrt::hstring SongItemViewModel::SongName()
    {
        return m_songName;
    }

    winrt::hstring SongItemViewModel::Singer()
    {
        return m_singer;
    }

    winrt::hstring SongItemViewModel::Mapper()
    {
        return m_mapper;
    }


    winrt::hstring SongItemViewModel::LengthString()
    {
        return Utils::GetDurationString(Utils::GetDuration(m_length));
    }

    int SongItemViewModel::Length()
    {
        return m_length;
    }

    int SongItemViewModel::Index()
    {
        return m_index;
    }

    void SongItemViewModel::SongName(winrt::hstring songName)
    {
        m_songName = songName;
    }

    void SongItemViewModel::Singer(winrt::hstring singer)
    {
        m_singer = singer;
    }

    void SongItemViewModel::Mapper(winrt::hstring mapper)
    {
        m_mapper = mapper;
    }

    void SongItemViewModel::Length(int length)
    {
        m_length = length;
    }

    void SongItemViewModel::Index(int index)
    {
        m_index = index;
    }

    winrt::Windows::Foundation::Collections::IObservableVector<winrt::hstring> SongItemViewModel::Versions()
    {
        //auto versionNames = winrt::single_threaded_observable_vector<winrt::hstring>();
        //for (auto const& versionFile : m_model.VersionFiles())
        //{
        //    versionNames.Append(
        //        winrt::to_hstring(OsuFile::ParseVersionFrom(std::string_view{ winrt::to_string(versionFile.Name()) }))
        //    );
        //}
        return m_versions;
    }

    //winrt::Windows::Media::Core::MediaSource SongItemViewModel::SongFile()
    //{
    //    return m_model.Source();
    //}

    //winrt::Windows::UI::Xaml::Media::Imaging::BitmapImage SongItemViewModel::Image()
    //{
    //    return winrt::Windows::UI::Xaml::Media::Imaging::BitmapImage();
    //}
    void SongItemViewModel::PlayCurrent()
    {
    }
    winrt::event_token SongItemViewModel::PropertyChanged(winrt::Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
    {
        return m_propertyChanged.add(handler);
    }
    void SongItemViewModel::PropertyChanged(winrt::event_token const& token) noexcept
    {
        m_propertyChanged.remove(token);
    }
}
