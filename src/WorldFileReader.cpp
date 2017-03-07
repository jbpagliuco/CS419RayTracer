#include "WorldFileReader.h"

WorldFileReader::WorldFileReader()
{
	groups["("] = ")";
	groups["["] = "]";
	groups["<"] = ">";
}

WorldFileReader::~WorldFileReader()
{

}

void WorldFileReader::AddRule(const std::string& name, std::vector<std::string> params, TypeRuleCB callback)
{
	rules[name] = Type(params, callback);
}