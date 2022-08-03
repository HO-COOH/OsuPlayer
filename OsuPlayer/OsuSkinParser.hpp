#pragma once

#include <fstream>
#include <string>
#include <string_view>
#include <Utils.FileStreamAdaptor.hpp>

namespace Osu
{
	struct SkinInfo
	{

		int majorVersion = 1;
		int minorVersion = 0;

		std::string name;

	private:
		void readInfo(std::istream& is)
		{
			constexpr std::string_view Flag = "Name: ";
			std::string line;
			while (std::getline(is, line))
			{
				if (auto index = line.find(Flag); index != std::string::npos)
				{
					name = line.substr(index + Flag.size());
					break;
				}
			}
		}

	public:

		SkinInfo(std::istream& is)
		{
			readInfo(is);
		}

		SkinInfo(winrt::Windows::Storage::StorageFile skinIni)
		{
			Utils::StreambufAdaptor buf{ skinIni };
			std::istream s{ &buf };
			readInfo(s);
		}
	};
}
