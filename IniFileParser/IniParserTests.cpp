#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include "IniParser.h"

TEST_CASE("Нахождение параметра", "[IniParser]") {

	SECTION("std::string")
	{
		std::unique_ptr<std::istream> stream = std::make_unique<std::istringstream>("[section1]\r\nparam1=value1");
		IniParser parser(std::move(stream));
		auto value = parser.getValue<std::string>("section1.param1");
		CHECK(value == "value1");
	}

	SECTION("int")
	{
		std::unique_ptr<std::istream> stream = std::make_unique<std::istringstream>("[section1]\r\nparam1=8");
		IniParser parser(std::move(stream));
		auto value = parser.getValue<int>("section1.param1");
		CHECK(value == 8);
	}

	SECTION("double")
	{
		std::unique_ptr<std::istream> stream = std::make_unique<std::istringstream>("[section1]\r\nparam1=1.2");
		IniParser parser(std::move(stream));
		auto value = parser.getValue<double>("section1.param1");
		CHECK(value == 1.2);
	}
}