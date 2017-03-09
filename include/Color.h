#pragma once

#include <Util.h>

namespace RE
{
	struct ColorU8
	{
		ColorU8() : r(0), g(0), b(0), a(0) { }
		ColorU8(U8 _r, U8 _g, U8 _b, U8 _a) : r(_r), g(_g), b(_b), a(_a) { }

		U8 r, g, b, a;
	};

	struct Color
	{
		Color() : r(0.0f), g(0.0f), b(0.0f), a(0.0f) { }
		Color(F32 _r, F32 _g, F32 _b, F32 _a) : r(_r), g(_g), b(_b), a(_a) { }
		Color(ColorU8);

		ColorU8 ToColorU8()const;

		Color operator+(const Color& other)const;
		Color& operator+=(const Color& other);

		Color operator*(const Color& other)const;
		Color operator*(F32 scalar)const;
		Color operator/(F32 d)const;

		void Clamp();

		F32 r, g, b, a;
	};

	void LoadColorFromConfigFile(const std::string&, const std::string& value, void *pLocation);
}