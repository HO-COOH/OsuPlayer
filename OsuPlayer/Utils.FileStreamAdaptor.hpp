#pragma once

#include <fstream>
#include <streambuf>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Storage.Streams.h>

namespace Utils
{
	template<typename Char = char>
	struct StreambufAdaptor :
		public std::basic_streambuf<Char>,
		private winrt::Windows::Storage::Streams::IBuffer
	{
		StreambufAdaptor(winrt::Windows::Storage::StorageFile file) :
			winrt::Windows::Storage::Streams::IBuffer{ winrt::Windows::Storage::FileIO::ReadBufferAsync(file).get() }
		{
			auto const beginPtr = reinterpret_cast<Char*>(data());
			std::basic_streambuf<Char>::setg(beginPtr, beginPtr, beginPtr + Length() / sizeof(Char));
		}

		std::streampos seekoff(typename std::basic_streambuf<Char>::off_type off, std::ios_base::seekdir dir, std::ios_base::openmode _) override
		{
			auto const beginPtr = reinterpret_cast<Char*>(data());
			auto const endPtr = beginPtr + Length() / sizeof(Char);
			auto const size = Length() / sizeof(Char);

			switch (dir)
			{
				case std::ios_base::beg:
					std::basic_streambuf<Char>::setg(beginPtr, beginPtr + off, endPtr);
					break;
				case std::ios_base::cur:
					std::basic_streambuf<Char>::setg(beginPtr, std::basic_streambuf<Char>::gptr() + off, endPtr);
					break;
				case std::ios_base::end:
					std::basic_streambuf<Char>::setg(beginPtr, endPtr + off, endPtr);
					break;
				default:
					assert(false);	//unknown dir?
			}
			return std::basic_streambuf<Char>::gptr() - std::basic_streambuf<Char>::eback();
		}

		auto getBuffer() const
		{
			return data();
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

}