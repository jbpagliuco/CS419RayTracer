#include "Util.h"

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <chrono>
#include <sstream>

namespace RE
{
	void LoadColorFromConfigFile(const std::string& key, const std::string& value, void *pLocation)
	{
		std::stringstream ss(value);

		std::string r, g, b, a;
		ss >> r >> g >> b >> a;

		Color * pColor = ((Color*)pLocation);
		pColor->r = std::stof(r);
		pColor->g = std::stof(g);
		pColor->b = std::stof(b);
		pColor->a = std::stof(a);
	}


	std::vector<std::string> SplitString(std::string s, const std::string &delim)
	{
		std::vector<std::string> v;

		size_t pos;
		while ((pos = s.find(delim)) != std::string::npos)
		{
			std::string arg = s.substr(0, pos);
			v.push_back(arg);
			s.erase(0, pos + delim.length());
		}
		v.push_back(s);

		return v;
	}

	void RemoveWhitespace(std::string &s)
	{
		for (std::string::iterator it = s.begin(); it != s.end(); it++)
		{
			char c = (*it);
			if (c == ' ' || c == '\n' || c == '\r' || c == '\t')
				s.erase(it);
		}
	}

	void RemoveCapWhitespace(std::string &s)
	{
		RemoveLeadingWhitespace(s);
		RemoveTrailingWhitespace(s);
	}

	void RemoveLeadingWhitespace(std::string &s)
	{
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	}

	void RemoveTrailingWhitespace(std::string &s)
	{
		s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	}


	std::chrono::high_resolution_clock::time_point GetTime()
	{
		return std::chrono::high_resolution_clock::now();
	}

	U64 DiffTime(const std::chrono::high_resolution_clock::time_point& start, const std::chrono::high_resolution_clock::time_point& end)
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	}



	ColorU8 Color::ToColorU8()const
	{
		return ColorU8(
			(U8)(std::max(0.0f, std::min(1.0f, r)) * 255.0f),
			(U8)(std::max(0.0f, std::min(1.0f, g)) * 255.0f),
			(U8)(std::max(0.0f, std::min(1.0f, b)) * 255.0f),
			(U8)(std::max(0.0f, std::min(1.0f, a)) * 255.0f)
		);
	}

	Color Color::operator+(const Color& other)const
	{
		return Color(
			r + other.r,
			g + other.g,
			b + other.b,
			a + other.a
		);
	}

	Color Color::operator*(const Color& other)const
	{
		return Color(
			r * other.r,
			g * other.g,
			b * other.b,
			a * other.a
		);
	}

	Color& Color::operator+=(const Color& other)
	{
		r += other.r;
		g += other.g;
		b += other.b;
		a += other.a;

		return *this;
	}

	Color Color::operator*(F32 scalar)const
	{
		return Color(
			r * scalar,
			g * scalar,
			b * scalar,
			a * scalar
		);
	}

	Color Color::operator/(F32 d)const
	{
		return Color(
			r / d,
			g / d,
			b / d,
			a / d
		);
	}

	void Color::Clamp()
	{
		r = std::max(0.0f, std::min(1.0f, r));
		g = std::max(0.0f, std::min(1.0f, g));
		b = std::max(0.0f, std::min(1.0f, b));
		a = std::max(0.0f, std::min(1.0f, a));
	}
}