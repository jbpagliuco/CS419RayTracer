#pragma once

#include <stdint.h>
#include <string>
#include <vector>
#include <chrono>

#define RE_SAFE_FREE(p) if (p) { delete p; p = nullptr; }
#define RE_SAFE_FREE_A(p) if (p) { delete[] p; p = nullptr; }
#define RE_SAFE_FREE_F(p, f) if (p) { f(p); p = nullptr; } 

#ifndef RE_ALIGN_MS
#if _MSC_VER && !__INTEL_COMPILER
#define RE_ALIGN_MS(alignment) __declspec(align(alignment))
#else
#define RE_ALIGN_MS(alignment)
#endif
#endif

#ifndef RE_ALIGN_GCC
#if __GNUC__
#define RE_ALIGN_GCC(alignment) __attribute__((aligned(alignment)))
#else
#define RE_ALIGN_GCC(alignment)
#endif
#endif

namespace RE
{
	typedef uint8_t byte;

	typedef int8_t I8;
	typedef uint8_t U8;

	typedef int16_t I16;
	typedef uint16_t U16;

	typedef int32_t I32;
	typedef uint32_t U32;

	typedef int64_t I64;
	typedef uint64_t U64;

	typedef float F32;
	typedef long double D64;

	const D64 INV_PI = 0.31830988618379067154;

	/* Holds 2 components */
	template <typename T>
	struct Vector2
	{
		Vector2<T>() {}
		Vector2<T>(T x, T y) { X = x; Y = y; }
		T X;
		T Y;
	};

	/* Holds 3 components */
	template <typename T>
	struct Vector3
	{
		Vector3<T>() {}
		Vector3<T>(T x, T y, T z) { X = x; Y = y; Z = z; }
		T X;
		T Y;
		T Z;
	};

	/* Holds 4 components */
	template <typename T>
	struct Vector4
	{
		Vector4<T>() {}
		Vector4<T>(T x, T y, T z, T w) { X = x; Y = y; Z = z; W = w; }
		T X;
		T Y;
		T Z;
		T W;
	};


	// Splits a string by delim
	std::vector<std::string> SplitString(std::string s, const std::string &delim);

	// Removes the whitespace in the string.
	// Includes: " ", "\n", "\t", "\r"
	void RemoveWhitespace(std::string &s);
	// Removes the whitespace at the beginning and end of the string.
	// Includes: " ", "\n", "\t", "\r"
	void RemoveCapWhitespace(std::string &s);
	// Removes the whitespace at the beginning of the string.
	// Includes: " ", "\n", "\t", "\r"
	void RemoveLeadingWhitespace(std::string &s);
	// Removes the whitespace at the end of the string.
	// Includes: " ", "\n", "\t", "\r"
	void RemoveTrailingWhitespace(std::string &s);

	// Returns the time, using a high resolution clock.
	// Use DiffTime() to get elapsed time.
	std::chrono::high_resolution_clock::time_point GetTime();
	// Calculates time difference.
	U64 DiffTime(const std::chrono::high_resolution_clock::time_point& start, const std::chrono::high_resolution_clock::time_point& end);



	I32 RandInt();
	I32 RandInt(I32 l, I32 h);
	F32 RandFloat();
	F32 RandFloat(F32 l, F32 h);







	inline I32 RandInt()
	{
		return rand();
	}

	inline I32 RandInt(I32 l, I32 h)
	{
		return (I32)(RandFloat(0.0f, (F32)(h - l + 1)) + l);
	}

	inline F32 RandFloat()
	{
		return (F32)(RandInt() * (1.0 / (F32)RAND_MAX));
	}

	inline F32 RandFloat(F32 l, F32 h)
	{
		return RandFloat() * (h - l) + l;
	}
}