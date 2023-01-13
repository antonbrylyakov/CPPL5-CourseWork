#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <sstream>
#include "Reader.h"
#include "IniFileParserExports.h"
#include "ReadParamError.h"

class INIFILEPARSER_API IniParser
{
public:
	IniParser(const std::string& fileName);
	IniParser(std::shared_ptr<std::istream> is);
	template <typename T> T getValue(const std::string& path)
	{
		auto rawValue = getRawValue(path);
		std::stringstream s(rawValue);
		T res;
		if (s >> res)
		{
			return res;
		}
		else
		{
			std::stringstream msg;
			msg << "Значение '" << rawValue << "' невозможно преобразовать к типу " << typeid(T).name();
			throw ReadParamError(msg.str());
		}
	}

	template <> std::string getValue<std::string>(const std::string& path)
	{
		return getRawValue(path);
	}

private:
	bool m_initialized = false;
	std::string m_fileName;
	std::shared_ptr<std::istream> m_is;
	std::unordered_map<std::string, std::unordered_map<std::string, std::string>> m_rawValues;
	std::string getRawValue(const std::string& path);
	void initialize();
	void initializeFromStream(std::shared_ptr<std::istream> is);
};
