#pragma once
#include "StringUtils.h"

namespace StringUtils
{
	std::string trimLeft(const std::string& s, const std::string trimChars) {
		auto start = s.find_first_not_of(trimChars);
		return (start == std::string::npos) ? "" : s.substr(start);
	}

	std::string trimRight(const std::string& s, const std::string trimChars) {
		auto end = s.find_last_not_of(trimChars);
		return (end == std::string::npos) ? "" : s.substr(0, end + 1);
	}

	std::string trim(const std::string& s, const std::string trimChars) {
		return trimRight(trimLeft(s, trimChars), trimChars);
	}
}