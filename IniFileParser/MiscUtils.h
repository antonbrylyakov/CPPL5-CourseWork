#pragma once
#include <string>
#include <ostream>

namespace MiscUtils
{
	template <typename T>
	void printKeys(const T& container, std::ostream& os)
	{
		bool first = true;
		auto it = container.cbegin();
		while (it != container.cend())
		{
			if (first)
			{
				first = false;
			}
			else
			{
				os << ", ";
			}
			os << it->first;
			++it;
		}
	}

}