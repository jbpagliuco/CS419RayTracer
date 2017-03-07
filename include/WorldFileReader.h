#pragma once

#include <string>
#include <vector>
#include <map>
#include "Util.h"

typedef std::map<std::string, std::string> TypeParams;
typedef void (*TypeRuleCB)(const std::string&, TypeParams, void* pWorld);

struct Type
{
	Type() {}
	Type(std::vector<std::string> params, TypeRuleCB callback) :
		params(params), callback(callback) { }

	std::vector<std::string> params;
	TypeRuleCB callback;
};

class WorldFileReader
{
public:
	WorldFileReader();

	virtual ~WorldFileReader();

	void AddRule(const std::string& name, std::vector<std::string> params, TypeRuleCB callback);

	void ReadFromFile(const std::string& file, void * pWorld);

private:
	TypeParams ReadParams(const Type& type, std::ifstream& in, std::stringstream& ss);
	std::string ReadParam(std::stringstream& ss, bool bCanBeUngrouped);

private:
	std::map<std::string, Type> rules;

	// Maps begin - end groups
	// ie "(" to ")"
	//	  "[" to "]"
	std::map<std::string, std::string> groups;
};