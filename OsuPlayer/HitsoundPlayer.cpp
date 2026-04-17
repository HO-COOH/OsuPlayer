#include "pch.h"
#include "HitsoundPlayer.h"
#include "ViewModelLocator.h"
#include "OsuParser.hpp"

int HitsoundPlayer::getHitsoundVolume(HitObject const& hitObject, TimingPoint const* timingPoint)
{
    if (hitObject.hitSample.volume == 0)
    {
        assert(timingPoint != nullptr);
        return timingPoint->volume;
    }
    return hitObject.hitSample.volume;
}

void HitsoundPlayer::addInputNode(winrt::Windows::Media::Audio::AudioFileInputNode node)
{
    //node.OutgoingGain(hitObject.hitSample.volume / 100.0);
    node.AddOutgoingConnection(m_outputNode);
    node.FileCompleted(
        [this](winrt::Windows::Media::Audio::AudioFileInputNode node, winrt::Windows::Foundation::IInspectable object)
        {
            node.RemoveOutgoingConnection(m_outputNode);
            node.Close();
            //We do NOT need to erase the node from a std::unordered_set, as this handler still get executed
        }
    );
    if (!m_hasStart)
        m_audioGraph.Start();
    node.Start();
}

winrt::Windows::Foundation::IAsyncAction HitsoundPlayer::addInputNode(winrt::Windows::Storage::StorageFile file)
{
    if (auto result = co_await m_audioGraph.CreateFileInputNodeAsync(file);
        result.Status() == winrt::Windows::Media::Audio::AudioFileNodeCreationStatus::Success)
    {
        addInputNode(result.FileInputNode());
    }
}

winrt::Windows::Foundation::IAsyncAction HitsoundPlayer::init()
{
    if (m_hasInit)
        co_return;

    winrt::Windows::Media::Audio::AudioGraphSettings settings{ winrt::Windows::Media::Render::AudioRenderCategory::GameEffects };
    m_audioGraph = (co_await winrt::Windows::Media::Audio::AudioGraph::CreateAsync(settings)).Graph();
    m_outputNode = (co_await m_audioGraph.CreateDeviceOutputNodeAsync()).DeviceOutputNode();
    m_outputNode.OutgoingGain(static_cast<double>(m_volume) / 100.0);
    m_hasInit = true;
}

winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Storage::StorageFile> HitsoundPlayer::getHitsoundFile(
    HitObject const& hitObject, 
    HitObject::HitSound sound,
    TimingPoint const* timingPoint)
{
    static auto hitsoundPanelViewModel = ViewModelLocator::Current().HitsoundPanelViewModel();

    auto const sampleSet = sound == HitObject::HitSound::Normal
        ? (hitObject.hitSample.normalSet == SampleSet::None ? timingPoint->sampleSet : hitObject.hitSample.normalSet)
        : (hitObject.hitSample.additionSet == SampleSet::None
            ? (hitObject.hitSample.normalSet == SampleSet::None ? timingPoint->sampleSet : hitObject.hitSample.normalSet)
            : hitObject.hitSample.additionSet);

    if (hitObject.hitSample.index == 0)
    { 
        //use skin hitsound
        switch (sampleSet)
        {
            case SampleSet::Normal:
                switch (sound)
                {
                    case HitObject::HitSound::Normal:	co_return hitsoundPanelViewModel.Normal().Hitnormal().File();
                    case HitObject::HitSound::Whistle:	co_return hitsoundPanelViewModel.Normal().Hitwhistle().File();
                    case HitObject::HitSound::Finish:	co_return hitsoundPanelViewModel.Normal().Hitfinish().File();
                    case HitObject::HitSound::Clap:		co_return hitsoundPanelViewModel.Normal().Hitclap().File();
                }
            case SampleSet::Soft:
                switch (sound)
                {
                    case HitObject::HitSound::Normal:	co_return hitsoundPanelViewModel.Soft().Hitnormal().File();
                    case HitObject::HitSound::Whistle:	co_return hitsoundPanelViewModel.Soft().Hitwhistle().File();
                    case HitObject::HitSound::Finish:	co_return hitsoundPanelViewModel.Soft().Hitfinish().File();
                    case HitObject::HitSound::Clap:		co_return hitsoundPanelViewModel.Soft().Hitclap().File();
                }

            case SampleSet::Drum:
                switch (sound)
                {
                    case HitObject::HitSound::Normal:	co_return hitsoundPanelViewModel.Drum().Hitnormal().File();
                    case HitObject::HitSound::Whistle:	co_return hitsoundPanelViewModel.Drum().Hitwhistle().File();
                    case HitObject::HitSound::Finish:	co_return hitsoundPanelViewModel.Drum().Hitfinish().File();
                    case HitObject::HitSound::Clap:		co_return hitsoundPanelViewModel.Drum().Hitclap().File();
                }
            default: co_return nullptr;
        }
    }
    else
    {
        //use beatmap hitsound
        std::wstring name;
        switch (sampleSet)
        {
            case SampleSet::Normal: name += L"normal-hit";  break;
            case SampleSet::Soft:   name += L"soft-hit";    break;
            case SampleSet::Drum:   name += L"drum-hit";    break;
            default:    assert(false);  //Auto should be handled before!
        }
        switch (sound)
        {
            case HitObject::HitSound::Normal:   name += L"normal";  break;
            case HitObject::HitSound::Whistle:  name += L"whistle"; break;
            case HitObject::HitSound::Finish:   name += L"finish";  break;
            case HitObject::HitSound::Clap:     name += L"clap";    break;
            default:    co_return nullptr;
        }
        name += std::to_wstring(hitObject.hitSample.index);
        name += L".wav";

        co_return co_await m_beatmapFolder.GetFileAsync(name);
    }
    co_return { nullptr };
}

static inline void AssertVolumeRange(int volume)
{
    assert(volume >= 1 && volume <= 100);
}


/* 
    Do I really need a std::unordered_set to keep the FileInputNode alive ?
    Nope, does not seems so. The FileCompleted() handler still get executed.
*/
void HitsoundPlayer::playHitsound(HitObject const& hitObject, TimingPoint const* timingPoint)
{
    if (!hitObject.hitSample.filename.empty())
    {
        addInputNode(m_beatmapFolder.GetFileAsync(winrt::to_hstring(hitObject.hitSample.filename)).get());
        return;
    }

    auto const hitsounds = hitObject.effectiveHitSounds();
    for (auto const sound : { HitObject::HitSound::Normal, HitObject::HitSound::Whistle, HitObject::HitSound::Finish, HitObject::HitSound::Clap })
    {
        if (!hitsounds.test(sound))
            continue;

        getHitsoundFile(hitObject, sound, timingPoint).Completed(
        [this](winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Storage::StorageFile> normalHitsoundAsync, winrt::Windows::Foundation::AsyncStatus status)
        {
            if (auto file = normalHitsoundAsync.GetResults(); file && status == winrt::Windows::Foundation::AsyncStatus::Completed)
                addInputNode(file);
        }
        );
    }
}

double HitsoundPlayer::NormalizedVolume()
{
    return m_volume;
}

void HitsoundPlayer::NormalizedVolume(double volume)
{
    if (m_hasInit)
        m_outputNode.OutgoingGain(volume);
    else
        m_volume = volume;
}
