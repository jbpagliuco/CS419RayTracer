#include "Util.h"

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <chrono>
#include <sstream>

namespace RE
{
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
}