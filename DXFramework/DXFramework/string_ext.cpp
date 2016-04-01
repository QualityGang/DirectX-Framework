#include "stdafx.h"
#include "string_ext.h"

void stde::string_split(const std::string &str, const std::string &delimiter, std::vector<std::string> *strings)
{
	strings->clear();

	std::string::size_type pos = 0;
	std::string::size_type prev = 0;

	while ((pos = str.find(delimiter, prev)) != std::string::npos)
	{
		strings->push_back(str.substr(prev, pos - prev));
		prev = pos + delimiter.size();
	}

	strings->push_back(str.substr(prev));
}