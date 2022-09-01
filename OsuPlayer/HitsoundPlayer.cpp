#include "pch.h"
#include "HitsoundPlayer.h"
#include "ViewModelLocator.h"
#include "OsuParser.hpp"

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


/* 
    Do I really need a std::unordered_set to keep the FileInputNode alive ?
    Nope, does not seems so. The FileCompleted() handler still get executed.
*/
winrt::Windows::Foundation::IAsyncAction HitsoundPlayer::playHitsound(HitObject const& hitObject)
{
    winrt::Windows::Storage::StorageFile source{ nullptr };
    auto hitsoundPanelViewModel = ViewModelLocator::Current().HitsoundPanelViewModel();
    switch (hitObject.hitSound)
    {
        case HitObject::HitSound::Normal:	source = hitsoundPanelViewModel.Normal().Hitnormal().File();	break;
        case HitObject::HitSound::Whistle:	source = hitsoundPanelViewModel.Normal().Hitwhistle().File(); break;
        case HitObject::HitSound::Finish:	source = hitsoundPanelViewModel.Normal().Hitfinish().File(); break;
        case HitObject::HitSound::Clap:		source = hitsoundPanelViewModel.Normal().Hitclap().File(); break;
    }
    if (!source)
        co_return;

    auto result = co_await m_audioGraph.CreateFileInputNodeAsync(source);
    if (result.Status() != winrt::Windows::Media::Audio::AudioFileNodeCreationStatus::Success)
        co_return;

    auto node = result.FileInputNode();
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
    //Similarly, we do NOT need to add the node to a std::unordered_set
}

int HitsoundPlayer::Volume()
{
    return static_cast<int>(m_outputNode.OutgoingGain() / 1.0);
}

void HitsoundPlayer::Volume(int volume)
{
    if (m_hasInit)
        m_outputNode.OutgoingGain(static_cast<double>(volume) / 100.0);
    else
        m_volume = volume;
}
