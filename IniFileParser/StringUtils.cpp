#pragma once
#include "StringUtils.h"
#include <algorithm>
#include <functional>

namespace StringUtils
{
	std::string trimLeft(const std::string& s, const std::string& trimChars) {
		auto start = s.find_first_not_of(trimChars);
		return (start == std::string::npos) ? "" : s.substr(start);
	}

	std::string trimRight(const std::string& s, const std::string& trimChars) {
		auto end = s.find_last_not_of(trimChars);
		return (end == std::string::npos) ? "" : s.substr(0, end + 1);
	}

	std::string trim(const std::string& s, const std::string& trimChars) {
		return trimRight(trimLeft(s, trimChars), trimChars);
	}

	std::string toLower(const std::string& s)
	{
		std::string res(s.size(), 0);
		std::transform(s.cbegin(),s.cend(), res.begin(), std::tolower);
		return res;
	}

	bool equalToCi::operator()(const std::string& lhs, const std::string& rhs) const
	{
		if (lhs.length() != rhs.length())
		{
			return false;
		}

		auto itl = lhs.cbegin(); 
		auto itr = rhs.cbegin();

		while (itl != lhs.cend() && itr != rhs.cend())
		{
			if (std::tolower(*itl) != std::tolower(*itr))
			{
				return false;
			}

			++itl;
			++itr;
		}

		return true;
	}
	std::size_t hashCi::operator()(const std::string& s) const
	{
		const auto sl = toLower(s);
		std::hash<std::string> h;
		return h(sl);
	}
}