#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <sstream>
#include "Reader.h"
#include "IniFileParserExports.h"
#include "ReadParamError.h"
#include "StringUtils.h"


// Класс - парсер ini-файлов.
// Особенности:
// - Названия секций и параметров нечувствительны к регистру
// - Не поддерживается Unicode
class INIFILEPARSER_API IniParser
{
public:
	// Конструктор для чтения из файла
	IniParser(const std::string& fileName);
	// Конструктор для чтения из потока
	IniParser(std::shared_ptr<std::istream> is);
	// Конструктор копирования и оператор копирующего присваивания удалены, т.к. в общем случае поток мы не копируем
	IniParser(const IniParser&) = delete;
	IniParser& operator =(const IniParser&) = delete;
	IniParser(IniParser&& ) = default;
	IniParser& operator =(IniParser&&) = default;

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
	// Словарь для хранения данных. Считаем, что названия секций и параметров нечувствителны к регистру
	std::unordered_map<
		std::string, 
		std::unordered_map<
			std::string, 
			std::string, 
			StringUtils::hashCi, 
			StringUtils::equalToCi>,
		StringUtils::hashCi,
		StringUtils::equalToCi> m_rawValues;
	std::string getRawValue(const std::string& path);
	void initialize();
	void initializeFromStream(std::shared_ptr<std::istream> is);
};
