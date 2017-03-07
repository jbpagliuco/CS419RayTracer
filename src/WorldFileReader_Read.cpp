#include "WorldFileReader.h"

#include <fstream>
#include <iostream>
#include <sstream>



std::string WorldFileReader::ReadParam(std::stringstream& ss, bool bCanBeUngrouped)
{
	std::string first;
	ss >> first;
	
	std::string firstChar = std::string(1, first[0]);
	if (groups.find(firstChar) != groups.end())
	{
		// Read a group of params

		// End character
		char endChar = groups[firstChar][0];
		// Output
		std::string out = "";

		// "(asdf ...)" vs "( asdf ...)"
		if (first.length() > 1)
			out += first.substr(1) + " ";

		while (true)
		{
			std::string curr;
			ss >> curr;

			if (curr[0] == endChar)
				return out;
			else if (curr[curr.length() - 1] == endChar)
				return out + curr.substr(0, curr.length() - 1);
			else
				out += curr + " ";
		}
	}
	else
	{	
		if (bCanBeUngrouped)
		{
			// We can just read everything as one group (even though it's not surrounded by group chars)
			std::string curr, out = first;
			while (!(ss >> curr).eof())
				out += " " + curr;
			
			if (curr == "")
				return out;
			return out + " " + curr;
		}
		else
			return first; // Was just a single param
	}
}



TypeParams WorldFileReader::ReadParams(const Type& type, std::ifstream& in, std::stringstream& ss)
{
	TypeParams out;
	auto p = type.params;

	// Either spread out on multiple lines, with each name
	//  or all on one line.
	if ((ss >> std::ws).eof())
	{
		std::string line;
		while (std::getline(in, line))
		{
			std::string trimmed = line;
			RE::RemoveCapWhitespace(trimmed);
			
			if (trimmed == "")
				break;

			std::stringstream ss2(line);
			std::string name;
			ss2 >> name;

			out[name] = ReadParam(ss2, true);
		}
	}
	else
	{
		for (auto it = p.begin(); it != p.end(); it++)
		{
			std::string name = *it;
			out[name] = ReadParam(ss, false);
		}
	}

	return out;
}







void WorldFileReader::ReadFromFile(const std::string& file, void * pWorld)
{
	std::ifstream in(file);

	if (!in.is_open())
	{
		std::cout << "Error opening file" << std::endl;
		return;
	}

	std::string line;
	while (std::getline(in, line))
	{
		std::string trimmed = line;
		RE::RemoveCapWhitespace(trimmed);

		// If we're at an empty line or comment
		if (trimmed == "" || 
			(trimmed.find("//") == 0))
		{
			continue;
		}

		std::stringstream ss(line);
		
		std::string first;
		bool bDone = (ss >> first).eof();

		std::string name = "";
		if (first[0] == '@')
		{
			name = first.substr(1);
			ss >> first;
		}

		if (rules.find(first) != rules.end())
		{
			Type t = rules[first];
			TypeParams p = ReadParams(t, in, ss);
			t.callback(name, p, pWorld);
		}
	}

	in.close();
}