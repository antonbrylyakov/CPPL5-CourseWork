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

TEST_CASE("Ненахождение параметра", "[IniParser]") {

	SECTION("Ненахождение секции")
	{
		std::unique_ptr<std::istream> stream = std::make_unique<std::istringstream>("[section1]\r\nparam1=value1");
		IniParser parser(std::move(stream));
		try
		{
			parser.getValue<std::string>("section2.param1");
		}
		catch (ReadParamError& e)
		{
			std::string s(e.what());
			CHECK(s.find("Допустимые секции: 'section1'"));
		}

		SECTION("Ненахождение параметра")
		{
			std::unique_ptr<std::istream> stream = std::make_unique<std::istringstream>("[section1]\r\nparam1=value1");
			IniParser parser(std::move(stream));
			try
			{
				parser.getValue<std::string>("section1.param2");
			}
			catch (ReadParamError& e)
			{
				std::string s(e.what());
				CHECK(s.find("Допустимые параметры: 'param1'"));
			}
		}
	}
}

TEST_CASE("Невалидные значения", "[IniParser]") {

	SECTION("int")
	{
		std::unique_ptr<std::istream> stream = std::make_unique<std::istringstream>("[section1]\r\nparam1=value1");
		IniParser parser(std::move(stream));
		CHECK_THROWS_AS(parser.getValue<int>("section1.param1"), ReadParamError);
	}

	SECTION("double")
	{
		std::unique_ptr<std::istream> stream = std::make_unique<std::istringstream>("[section1]\r\nparam1=value1");
		IniParser parser(std::move(stream));
		CHECK_THROWS_AS(parser.getValue<double>("section1.param1"), ReadParamError);
	}
}