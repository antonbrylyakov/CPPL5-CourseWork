#include <catch2/catch_test_macros.hpp>
#include "StringUtils.h"

TEST_CASE("trim", "[StringUtils]") {

	SECTION("trimRight 1")
	{
		CHECK(StringUtils::trimRight("str \t") == "str");
	}

	SECTION("trimRight 2")
	{
		CHECK(StringUtils::trimRight("str \t;", " \t;") == "str");
	}

	SECTION("trimRight 3")
	{
		CHECK(StringUtils::trimRight("str") == "str");
	}

	SECTION("trimLeft 1")
	{
		CHECK(StringUtils::trimLeft(" \tstr") == "str");
	}

	SECTION("trimLeft 2")
	{
		CHECK(StringUtils::trimLeft(" \t;str", " \t;") == "str");
	}

	SECTION("trimLeft 3")
	{
		CHECK(StringUtils::trimLeft("str") == "str");
	}

	SECTION("trim 1")
	{
		CHECK(StringUtils::trim(" \tstr\t ") == "str");
	}

	SECTION("trim 2")
	{
		CHECK(StringUtils::trim("str") == "str");
	}

	SECTION("trim 3")
	{
		CHECK(StringUtils::trim("") == "");
	}

	SECTION("trim 4")
	{
		CHECK(StringUtils::trim("\t \t ") == "");
	}
}

TEST_CASE("toLower", "[StringUtils]") {

	SECTION("toLower 1")
	{
		CHECK(StringUtils::toLower("Str1") == "str1");
	}

	SECTION("toLower 2")
	{
		CHECK(StringUtils::toLower("ABCDEFGHIJKLMNOPQRSTUVWXYZ") == "abcdefghijklmnopqrstuvwxyz");
	}
}

TEST_CASE("equalsToCi", "[StringUtils]") {

	SECTION("equalsToCi 1")
	{
		CHECK(StringUtils::equalToCi()("Str1", "str1"));
	}

	SECTION("equalsToCi 2")
	{
		CHECK(!StringUtils::equalToCi()("Strx1", "str1"));
	}
}

TEST_CASE("hashCi", "[StringUtils]") {

	SECTION("hashCi 1")
	{
		StringUtils::hashCi h;
		CHECK(h("Str1") == h("str1"));
	}

	SECTION("hashCi 2")
	{
		StringUtils::hashCi h;
		CHECK(h("StringA1") != h("StringB1"));
	}
}