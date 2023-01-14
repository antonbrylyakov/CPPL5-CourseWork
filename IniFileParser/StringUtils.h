#pragma once
#include <string>

// Строковые утилиты
namespace StringUtils
{
	const std::string WHITESPACES = " \n\r\t";

	std::string trimLeft(const std::string& s, const std::string& trimChars = WHITESPACES);

	std::string trimRight(const std::string& s, const std::string& trimChars = WHITESPACES);

	std::string trim(const std::string& s, const std::string& trimChars = WHITESPACES);

	std::string toLower(const std::string& s);

	template <typename T>
	void split(const std::string& s, T& container, const char delimiter)
	{
		size_t itemBegin;
		size_t itemEnd = 0;
		while ((itemBegin = s.find_first_not_of(delimiter, itemEnd)) != std::string::npos)
		{
			itemEnd = s.find(delimiter, itemBegin);
			const auto item = s.substr(itemBegin, itemEnd - itemBegin);
			container.push_back(item);
		}
	}

	// Функтор сравнения строк без учета регистра для использования в словаре
	struct equalToCi
	{
		bool operator()(const std::string& lhs, const std::string& rhs) const;
	};

	// Функтор хэширования строк без учета регистра для использования в словаре
	struct hashCi
	{
		std::size_t operator()(const std::string& s) const;
	};
}