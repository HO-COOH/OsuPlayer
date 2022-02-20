#include "pch.h"
#include "CppUnitTest.h"
#include "../OsuPlayer/OsuDBParser.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Utils
{
	TEST_CLASS(Serialize)
	{
	public:
		TEST_METHOD(OsuDb)
		{
			Db::Osu osuDb{ std::ifstream{"osu!.db"} };
			auto originalBuffer = Db::LoadBinaryFromFile(std::ifstream{ "osu!.db" });
			

			//Assert::IsTrue(memcmp(originalBuffer.get(), ))
		}
	};
}
