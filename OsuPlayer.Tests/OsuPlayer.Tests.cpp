#include "pch.h"
#include "CppUnitTest.h"
#include "../OsuPlayer/OsuDBParser.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Utils
{
	TEST_CLASS(Serialize)
	{
		template<typename DbType>
		static auto GetDbAndOriginalBinary(std::string_view path)
		{
			return std::pair{ 
				DbType{ std::ifstream{path.data(), std::ios_base::binary} }, 
				Db::LoadBinaryFromFile(std::ifstream{path.data(), std::ios_base::binary}) 
			};
		}
	public:
		TEST_METHOD(OsuDb)
		{
			auto const [osuDb, originalBuffer] = GetDbAndOriginalBinary<Db::Osu>("osu!.db");
			Assert::IsTrue(memcmp(originalBuffer.get(), osuDb.getBuffer().get(), osuDb.bytes()), L"Serialized and deserialized osu!.db are not equal");
		}

		TEST_METHOD(ScoreDb)
		{
			auto const [scoreDb, originalBuffer] = GetDbAndOriginalBinary<Db::Scores>("scores.db");
			Assert::IsTrue(memcmp(scoreDb.getBuffer().get(), originalBuffer.get(), scoreDb.bytes()), L"Serialized and deserialized scores.db are not equal");
		}

		TEST_METHOD(CollectionDb)
		{
			auto const [collectionDb, originalBuffer] = GetDbAndOriginalBinary<Db::Collections>("collection.db");
			Assert::IsTrue(memcmp(collectionDb.getBuffer().get(), originalBuffer.get(), collectionDb.bytes()), L"Serialized and deserialized collection.db are not equal");
		}
	};
}
