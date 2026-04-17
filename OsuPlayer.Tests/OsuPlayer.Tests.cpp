#include "pch.h"
#include "../OsuPlayer/OsuParser.hpp"
#include "ToStringHelper.hpp"



using namespace Microsoft::VisualStudio::CppUnitTestFramework;

static std::filesystem::path GetTestDataPath(std::string_view fileName)
{
	return std::filesystem::path{ __FILE__ }.parent_path() / fileName;
}

namespace Utils
{


	//TEST_CLASS(Serialize)
	//{
	//	template<typename DbType>
	//	static auto GetDbAndOriginalBinary(std::string_view path)
	//	{
	//		return std::pair{ 
	//			DbType{ std::ifstream{path.data(), std::ios_base::binary} }, 
	//			Db::LoadBinaryFromFile(std::ifstream{path.data(), std::ios_base::binary}) 
	//		};
	//	}
	//public:
	//	TEST_METHOD(OsuDb)
	//	{
	//		auto const [osuDb, originalBuffer] = GetDbAndOriginalBinary<Db::Osu>("osu!.db");
	//		Assert::IsTrue(memcmp(originalBuffer.get(), osuDb.getBuffer().get(), osuDb.bytes()), L"Serialized and deserialized osu!.db are not equal");
	//	}

	//	TEST_METHOD(ScoreDb)
	//	{
	//		auto const [scoreDb, originalBuffer] = GetDbAndOriginalBinary<Db::Scores>("scores.db");
	//		Assert::IsTrue(memcmp(scoreDb.getBuffer().get(), originalBuffer.get(), scoreDb.bytes()), L"Serialized and deserialized scores.db are not equal");
	//	}

	//	TEST_METHOD(CollectionDb)
	//	{
	//		auto const [collectionDb, originalBuffer] = GetDbAndOriginalBinary<Db::Collections>("collection.db");
	//		Assert::IsTrue(memcmp(collectionDb.getBuffer().get(), originalBuffer.get(), collectionDb.bytes()), L"Serialized and deserialized collection.db are not equal");
	//	}
	//};

	TEST_CLASS(OsuFileParse)
	{
		OsuFile rankedMap{ std::ifstream{GetTestDataPath("Koda Kumi - Guess Who Is Back (TV Size) (Sotarks) [Fiery's Extreme].osu") }};
	public:
		TEST_METHOD(General)
		{
			auto const& general = rankedMap.general;
			Assert::AreEqual("audio.mp3", general.audioFile.data());
			Assert::AreEqual(22141, general.previewTime);
			Assert::AreEqual(static_cast<int>(Countdown::No), static_cast<int>(general.countdown));
			Assert::AreEqual(SampleSet::Soft, general.sampleSet);
			Assert::AreEqual(0.4f, general.stackLeniency);
			Assert::AreEqual(Mode::Osu, general.mode);
			Assert::AreEqual(false, general.letterboxInBreaks);
			Assert::AreEqual(true, general.wideScreenStoryboard);
		}

		TEST_METHOD(Editor)
		{
			auto const& editor = rankedMap.editor;
			Assert::AreEqual(1.f, editor.distanceSpacing);
			Assert::AreEqual(4.f, editor.beatDivisor);
			Assert::AreEqual(4, editor.gridSize);
			Assert::AreEqual(2.55f, editor.timelineZoom);
		}

		TEST_METHOD(Metadata)
		{
			auto const& metadata = rankedMap.metaData;
			Assert::AreEqual("Guess Who Is Back (TV Size)", metadata.title.data());
			Assert::AreEqual("Koda Kumi", metadata.artist.data());
			Assert::AreEqual("Sotarks", metadata.creator.data());
			Assert::AreEqual("Fiery's Extreme", metadata.version.data());
			Assert::AreEqual("ブラッククローバー", metadata.source.data());
			Assert::AreEqual(65ull, metadata.tags.size());
			Assert::AreEqual("Black", metadata.tags.front().data());
			Assert::AreEqual("stetson", metadata.tags.back().data());
			Assert::AreEqual(1911308, metadata.beatmapId);
			Assert::AreEqual(906786, metadata.beatmapSetId);
		}
		TEST_METHOD(Difficulty)
		{
			auto const& difficulty = rankedMap.difficulty;
			Assert::AreEqual(5.f, difficulty.HPDrainRate);
			Assert::AreEqual(4.2f, difficulty.circleSize);
			Assert::AreEqual(9.4f, difficulty.overallDifficulty);
			Assert::AreEqual(9.2f, difficulty.approachRate);
			Assert::AreEqual(1.8f, difficulty.sliderMultiplier);
			Assert::AreEqual(1, difficulty.sliderTickRate);
		}

		TEST_METHOD(Events)
		{
			auto const& events = rankedMap.events;
			Assert::AreEqual(2ull, events.size());

			auto const* video = events.first<Video>();
			Assert::IsNotNull(video);
			Assert::AreEqual(0, video->startTime);
			Assert::AreEqual("video.avi", video->fileName.data());

			auto const* background = events.first<Background>();
			Assert::IsNotNull(background);
			Assert::AreEqual("bg.jpg", background->fileName.data());
			Assert::AreEqual(0, background->startTime);
			Assert::AreEqual(0, background->xOffset);
			Assert::AreEqual(0, background->yOffset);
		}
		
		TEST_METHOD(TimingPoints)
		{
			auto const& timingPoints = rankedMap.timingPoints;
			Assert::AreEqual(17ull, timingPoints.size());

			//first
			{
				auto const& first = timingPoints.front();
				Assert::AreEqual(1240, first.time);
				Assert::AreEqual(327.869f, first.beatLength, 0.001f);
				Assert::AreEqual(4, first.meter);
				Assert::AreEqual(2, static_cast<int>(first.sampleSet));
				Assert::AreEqual(22, first.sampleIndex);
				Assert::AreEqual(42, first.volume);
				Assert::AreEqual(true, first.uninherited);
				Assert::AreEqual(0u, first.effects);
			}

			//last
			{
				auto const& last = timingPoints.back();
				Assert::AreEqual(85174, last.time);
				Assert::AreEqual(-100.f, last.beatLength);
				Assert::AreEqual(1.f * rankedMap.difficulty.sliderMultiplier, last.GetSliderVelocity(rankedMap.difficulty));
				Assert::AreEqual(4, last.meter);
				Assert::AreEqual(2, static_cast<int>(last.sampleSet));
				Assert::AreEqual(22, last.sampleIndex);
				Assert::AreEqual(72, last.volume);
				Assert::AreEqual(false, last.uninherited);
				Assert::AreEqual(0u, last.effects);
			}
		}

		TEST_METHOD(Colors)
		{
			auto const& colors = rankedMap.colors;
			Assert::AreEqual(4ull, colors.comboColor.size());
			{
				//first
				auto [r, g, b] = colors.comboColor.front();
				Assert::AreEqual(255, static_cast<int>(r));
				Assert::AreEqual(17, static_cast<int>(g));
				Assert::AreEqual(17, static_cast<int>(b));
			}
			{
				//last
				auto [r, g, b] = colors.comboColor.back();
				Assert::AreEqual(240, static_cast<int>(r));
				Assert::AreEqual(78, static_cast<int>(g));
				Assert::AreEqual(0, static_cast<int>(b));
			}
		}

		TEST_METHOD(HitObjects)
		{
			auto const& hitObjects = rankedMap.hitObjects;
			Assert::AreEqual(303ull, hitObjects.size());

			{
				//first slider
				auto const& first = hitObjects.front();
				Assert::IsTrue(std::holds_alternative<Slider>(first));
				auto const& slider = std::get<Slider>(first);
				Assert::AreEqual(220, slider.x);
				Assert::AreEqual(73, slider.y);
				Assert::AreEqual(1240, slider.time);
				Assert::IsTrue(slider.type.test(HitObject::Type::NewCombo));
				Assert::IsTrue(slider.hitSound.none());
				Assert::AreEqual(Slider::CurveType::Circle, slider.curveType);
				Assert::AreEqual(2ull, slider.curvePoints.size());
				Assert::AreEqual(Coord{ .x = 285, .y = 62 }, slider.curvePoints[0]);
				Assert::AreEqual(Coord{ .x = 352, .y = 84 }, slider.curvePoints[1]);
				Assert::AreEqual(2, slider.slides);
				Assert::AreEqual(135.f, slider.length);
				Assert::AreEqual(3ull, slider.edgeSounds.size());
				Assert::AreEqual(3ull, slider.edgeSets.size());
				Assert::AreEqual(
					std::vector
					{ 
						HitObject::HitSoundFlags{HitObject::HitSound::Whistle},
						HitObject::HitSoundFlags{HitObject::HitSound::Whistle},
						HitObject::HitSoundFlags{HitObject::HitSound::Whistle} 
					}, 
					slider.edgeSounds
				);
				Assert::AreEqual(
					std::vector
					{
						Slider::EdgeSet{0, 0},
						Slider::EdgeSet{0, 0},
						Slider::EdgeSet{0, 0}
					},
					slider.edgeSets
				);
				Assert::AreEqual(
					HitObject::HitSample{ 0,0,0,0 },
					slider.hitSample
				);
			}

			{
				//last circle
				auto const& last = hitObjects.back();
				Assert::IsTrue(std::holds_alternative<Circle>(last));
				auto const& circle = std::get<Circle>(last);
				Assert::AreEqual(341, circle.x);
				Assert::AreEqual(308, circle.y);
				Assert::AreEqual(87633, circle.time);
				Assert::IsTrue(circle.hitSound.test(HitObject::HitSound::Clap));
				Assert::IsTrue(circle.hitSound.test(HitObject::HitSound::Whistle));
				Assert::AreEqual(2, static_cast<int>(circle.hitSample.additionSet));
			}
		}
	};
}
