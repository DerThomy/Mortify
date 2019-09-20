#pragma once

#include "Log.h"

namespace Mortify
{
	struct Buffer
	{
		byte* Data;
		uint32_t Size;

		Buffer()
			: Data(nullptr), Size(0)
		{}

		Buffer(byte* data, uint32_t size)
			: Data(data), Size(size)
		{}

		void Allocate(uint32_t size)
		{
			delete[] Data;
			Data = nullptr;

			if (size == 0)
				return;

			Data = new byte[size];
			Size = size;
		}

		void ZeroInitialize()
		{
			if (Data)
				memset(Data, 0, Size);
		}

		void Write(byte* data, uint32_t size, uint32_t offset = 0)
		{
			MT_CORE_ASSERT(size + offset <= Size, "Buffer overflow!");
			memcpy(Data + offset, data, size);
		}

		operator bool() const
		{
			return Data;
		}

		byte& operator[](int index)
		{
			return Data[index];
		}

		byte operator[](int index) const
		{
			return Data[index];
		}

		template <typename T>
		T* As()
		{
			return (T*)Data;
		}

		inline uint32_t GetSize() const { return Size; }
	};
}