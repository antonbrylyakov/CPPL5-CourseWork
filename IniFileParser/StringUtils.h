#pragma once
#include <string>

namespace StringUtils
{
	const std::string WHITESPACES = " \n\r\t";

	std::string trimLeft(const std::string& s, const std::string trimChars = WHITESPACES);

	std::string trimRight(const std::string& s, const std::string trimChars = WHITESPACES);

	std::string trim(const std::string& s, const std::string trimChars = WHITESPACES);
}