#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include "Reader.h"

class IniParser
{
public:
	IniParser(const std::string& fileName);
	IniParser(std::unique_ptr<std::istream> is);
	//template <typename T> T getValue(const std::string& path);

private:
	bool m_initialized = false;
	Reader m_reader;
	std::unordered_map<std::string, std::unordered_map<std::string, std::string>> m_rawValues;
	//std::string getRawValue(const std::string& path);
	void initialize();
};

/*template <typename T> T IniParser::getValue(const std::string& path)
{
	
}*/