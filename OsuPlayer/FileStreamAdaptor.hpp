#pragma once

#include <fstream>
#include <streambuf>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Storage.Streams.h>

template<typename Char = char>
struct StreambufAdaptor : 
	public std::basic_streambuf<Char>, 
	private winrt::Windows::Storage::Streams::IBuffer
{
	StreambufAdaptor(winrt::Windows::Storage::StorageFile file) : 
		winrt::Windows::Storage::Streams::IBuffer{ winrt::Windows::Storage::FileIO::ReadBufferAsync(file).get() }
	{
		auto const beginPtr = reinterpret_cast<Char*>(data());
		setg(beginPtr, beginPtr, beginPtr + Length() / sizeof(Char));
	}
};

//template<typename Char = char>
//class StreambufAdaptor : public std::basic_streambuf<Char>
//{
//	winrt::Windows::Storage::Streams::IBuffer buffer;
//	//winrt::Windows::Storage::Streams::DataReader reader;
//public:
//	StreambufAdaptor(winrt::Windows::Storage::StorageFile file)
//		: buffer{ winrt::Windows::Storage::FileIO::ReadBufferAsync(file).get() }
//		//reader{winrt::Windows::Storage::Streams::DataReader::FromBuffer(buffer) }
//	{
//		auto const beginPtr = reinterpret_cast<Char*>(buffer.data());
//		//auto const length = buffer.Capacity() / sizeof(wchar_t);
//		setg(beginPtr, beginPtr, beginPtr + buffer.Length() / sizeof(Char));
//	}
//
//	//std::streamsize xsgetn(Char* s, std::streamsize count) override
//	//{
//	//	decltype(count) i = 0;
//	//	for (; i < count * sizeof(Char) && reader.UnconsumedBufferLength() > 0; ++i)
//	//	{
//	//		reinterpret_cast<Char*>(s)[i] = reader.ReadByte();
//	//	}
//	//	return i;
//	//}
//};