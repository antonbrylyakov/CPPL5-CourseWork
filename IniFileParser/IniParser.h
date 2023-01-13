#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <sstream>
#include "Reader.h"

class IniParser
{
public:
	IniParser(const std::string& fileName);
	IniParser(std::shared_ptr<std::istream> is);
	template <typename T> T getValue(const std::string& path);

private:
	bool m_initialized = false;
	std::string m_fileName;
	std::shared_ptr<std::istream> m_is;
	std::unordered_map<std::string, std::unordered_map<std::string, std::string>> m_rawValues;
	std::string getRawValue(const std::string& path);
	void initialize();
	void initializeFromStream(std::shared_ptr<std::istream> is);
};

template <typename T> T IniParser::getValue(const std::string& path)
{
	auto rawValue = getRawValue(path);
	std::stringstream s(rawValue);
	T res;
	s >> res;
	return res;
}

template <> std::string IniParser::getValue<std::string>(const std::string& path)
{
	return getRawValue(path);
}