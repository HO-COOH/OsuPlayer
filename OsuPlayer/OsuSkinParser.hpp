#pragma once

#include <fstream>
#include <string>

namespace Osu
{
	struct Skin
	{
		int majorVersion = 1;
		int minorVersion = 0;

		std::string name;

		Skin(std::istream& is)
		{
			constexpr auto Flag = "Name: ";
			std::string line;
			while (std::getline(is, line))
			{
				if (auto index = line.find(Flag); index != line.end())
				{
					name = line.substr(index + Flag.size());
					break;
				}
			}
		}
	};
}
