#pragma once

#define NOMINMAX
#include <Windows.h>
#include <sstream>

class ConsoleLogger
{
	std::wstringstream m_stream;
public:
	
	template<typename T>
	ConsoleLogger& operator<<(T&& data)
	{
		m_stream << data;
		return *this;
	}

	~ConsoleLogger()
	{
		auto const str = m_stream.str();
		OutputDebugString(str.data());
	}
};
