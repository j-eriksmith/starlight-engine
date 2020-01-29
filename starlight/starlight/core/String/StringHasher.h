// The MIT License(MIT)
// 
// Copyright(c) 2015 Stefan Reinalter
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#pragma once
#include <cstdint>
#include <string>

namespace _Internal
{
	template <size_t index>
	class Hash
	{
		static constexpr unsigned long long PRIME_MULTIPLE = 16777619ull;

	public:
		template <size_t strLen>
		constexpr static unsigned int Generate(char(&str)[strLen])
		{
			return static_cast<unsigned int>(static_cast<uint64>(Hash<index - 1u>::Generate(str) ^ unsigned int(str[index - 1u])) * PRIME_MULTIPLE);
		}
	};

	template <>
	class Hash<0u>
	{
		static constexpr unsigned int INITIAL_HASH = 2166136261u;
	public:
		template <size_t strLen>
		constexpr static unsigned int Generate(char(&str)[strLen])
		{
			return INITIAL_HASH;
		}
	};


	template <typename T>
	class HashHelper {};

	// Runtime Version
	/*
	template <>
	class HashHelper<const char*>
	{
	public:
		static unsigned int Generate(const char* str)
		{
			return Fnv1aHash(str);
		}
	private:
		static inline unsigned int Fnv1aHash(unsigned int hash, const char* str, size_t length)
		{
			for (size_t i = 0; i < length; ++i)
			{
				const unsigned int value = static_cast<unsigned int>(*str++);
				hash ^= value;
				hash *= 16777619u;
			}

			return hash;
		}

		static inline unsigned int Fnv1aHash(const char* str, size_t length)
		{
			return Fnv1aHash(2166136261u, str, length);
		}

		static inline unsigned int Fnv1aHash(const char* str)
		{
			return Fnv1aHash(str, strlen(str));
		}
	};
	*/

	// Compile-time version
	template <size_t strLen>
	class HashHelper<char[strLen]>
	{
	public:
		static constexpr unsigned int Generate(const char(&str)[strlen])
		{
			return Hash<strLen - 1u>::Generate(str);
		}
	};
}


// Calls the runtime or compile-time version of string hashing based on parameter type.
template <class Type>
constexpr unsigned int HashString(const Type& str)
{
	return _Internal::HashHelper<Type>::Generate(str);
}
