#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include "Reader.h"
#include "CommentEvent.h"
#include "SectionStartEvent.h"
#include "ParseError.h"
#include "ParameterValueEvent.h"

TEST_CASE("Нет событий на пустом файле", "[Reader]") {
	
	std::unique_ptr<std::istream> stream = std::make_unique<std::istringstream>("");
	Reader reader(std::move(stream));
	auto evt = reader.getNextEvent();
	CHECK(!evt);
}

TEST_CASE("Комментарий в отдельной строке", "[Reader]") {

	std::unique_ptr<std::istream> stream = std::make_unique<std::istringstream>(" ; comment text ");
	Reader reader(std::move(stream));
	auto evt = reader.getNextEvent();
	REQUIRE(typeid(*evt) == typeid(CommentEvent));
	auto typedEvent = dynamic_cast<CommentEvent*>(evt.get());
	CHECK(typedEvent->getText() == "comment text");
}

TEST_CASE("Начало секции", "[Reader]") {

	SECTION("Обычное название")
	{
		std::unique_ptr<std::istream> stream = std::make_unique<std::istringstream>("\t [section1] \t");
		Reader reader(std::move(stream));
		auto evt = reader.getNextEvent();
		REQUIRE(typeid(*evt) == typeid(SectionStartEvent));
		auto typedEvent = dynamic_cast<SectionStartEvent*>(evt.get());
		CHECK(typedEvent->getSectionName() == "section1");
	}

	SECTION("Название с пробелом в середине")
	{
		std::unique_ptr<std::istream> stream = std::make_unique<std::istringstream>("\t [section  name] \t");
		Reader reader(std::move(stream));
		auto evt = reader.getNextEvent();
		REQUIRE(typeid(*evt) == typeid(SectionStartEvent));
		auto typedEvent = dynamic_cast<SectionStartEvent*>(evt.get());
		CHECK(typedEvent->getSectionName() == "section  name");
	}

	SECTION("Пробелы в начале и в конце")
	{
		std::unique_ptr<std::istream> stream = std::make_unique<std::istringstream>("\t [ section name ] \t");
		Reader reader(std::move(stream));
		auto evt = reader.getNextEvent();
		REQUIRE(typeid(*evt) == typeid(SectionStartEvent));
		auto typedEvent = dynamic_cast<SectionStartEvent*>(evt.get());
		CHECK(typedEvent->getSectionName() == "section name");
	}

	SECTION("Секция с комментарием")
	{
		std::unique_ptr<std::istream> stream = std::make_unique<std::istringstream>("[section1];comment ");
		Reader reader(std::move(stream));
		auto evt = reader.getNextEvent();
		REQUIRE(typeid(*evt) == typeid(SectionStartEvent));
		auto typedEvent1 = dynamic_cast<SectionStartEvent*>(evt.get());
		CHECK(typedEvent1->getSectionName() == "section1");
		evt = reader.getNextEvent();
		REQUIRE(typeid(*evt) == typeid(CommentEvent));
		auto typedEvent2 = dynamic_cast<CommentEvent*>(evt.get());
		CHECK(typedEvent2->getText() == "comment");
		evt = reader.getNextEvent();
	}

	SECTION("Секция с комментарием на новой строке")
	{
		std::unique_ptr<std::istream> stream = std::make_unique<std::istringstream>("[section1]\r\n;comment ");
		Reader reader(std::move(stream));
		auto evt = reader.getNextEvent();
		REQUIRE(typeid(*evt) == typeid(SectionStartEvent));
		auto typedEvent1 = dynamic_cast<SectionStartEvent*>(evt.get());
		CHECK(typedEvent1->getSectionName() == "section1");
		evt = reader.getNextEvent();
		REQUIRE(typeid(*evt) == typeid(CommentEvent));
		auto typedEvent2 = dynamic_cast<CommentEvent*>(evt.get());
		CHECK(typedEvent2->getText() == "comment");
		evt = reader.getNextEvent();
	}

	SECTION("Незавершенная секция 1")
	{
		std::unique_ptr<std::istream> stream = std::make_unique<std::istringstream>("[section");
		Reader reader(std::move(stream));
		CHECK_THROWS_AS(reader.getNextEvent(), ParseError);
	}

	SECTION("Незавершенная секция 2")
	{
		std::unique_ptr<std::istream> stream = std::make_unique<std::istringstream>("[section [");
		Reader reader(std::move(stream));
		CHECK_THROWS_AS(reader.getNextEvent(), ParseError);
	}
}

TEST_CASE("Параметры", "[Reader]") {

	SECTION("Один параметр в секции")
	{
		std::unique_ptr<std::istream> stream = std::make_unique<std::istringstream>("[section1]\r\nparam1=value1");
		Reader reader(std::move(stream));
		auto evt = reader.getNextEvent();
		evt = reader.getNextEvent();
		REQUIRE(typeid(*evt) == typeid(ParameterValueEvent));
		auto typedEvent = dynamic_cast<ParameterValueEvent*>(evt.get());
		CHECK(typedEvent->getKey() == "param1");
		CHECK(typedEvent->getValue() == "value1");
	}

	SECTION("Два параметра в секции")
	{
		std::unique_ptr<std::istream> stream = std::make_unique<std::istringstream>("[section1]\r\nparam1=value1\rparam2=value2");
		Reader reader(std::move(stream));
		auto evt = reader.getNextEvent();
		evt = reader.getNextEvent();
		REQUIRE(typeid(*evt) == typeid(ParameterValueEvent));
		auto typedEvent1 = dynamic_cast<ParameterValueEvent*>(evt.get());
		CHECK(typedEvent1->getKey() == "param1");
		CHECK(typedEvent1->getValue() == "value1");
		evt = reader.getNextEvent();
		REQUIRE(typeid(*evt) == typeid(ParameterValueEvent));
		auto typedEvent2 = dynamic_cast<ParameterValueEvent*>(evt.get());
		CHECK(typedEvent2->getKey() == "param2");
		CHECK(typedEvent2->getValue() == "value2");
	}

	SECTION("Значение параметра с пробелами")
	{
		std::unique_ptr<std::istream> stream = std::make_unique<std::istringstream>("[section1]\r\nparam=some value ");
		Reader reader(std::move(stream));
		auto evt = reader.getNextEvent();
		evt = reader.getNextEvent();
		REQUIRE(typeid(*evt) == typeid(ParameterValueEvent));
		auto typedEvent = dynamic_cast<ParameterValueEvent*>(evt.get());
		CHECK(typedEvent->getKey() == "param");
		CHECK(typedEvent->getValue() == "some value ");
	}

	SECTION("Пробелы перед знаком равенства")
	{
		std::unique_ptr<std::istream> stream = std::make_unique<std::istringstream>("[section1]\r\nparam  \t=some value ");
		Reader reader(std::move(stream));
		auto evt = reader.getNextEvent();
		evt = reader.getNextEvent();
		REQUIRE(typeid(*evt) == typeid(ParameterValueEvent));
		auto typedEvent = dynamic_cast<ParameterValueEvent*>(evt.get());
		CHECK(typedEvent->getKey() == "param");
		CHECK(typedEvent->getValue() == "some value ");
	}

	SECTION("Комментарий в строке параметра")
	{
		std::unique_ptr<std::istream> stream = std::make_unique<std::istringstream>("[section1]\r\nparam=some value ; comment text");
		Reader reader(std::move(stream));
		auto evt = reader.getNextEvent();
		evt = reader.getNextEvent();
		REQUIRE(typeid(*evt) == typeid(ParameterValueEvent));
		auto typedEvent1 = dynamic_cast<ParameterValueEvent*>(evt.get());
		CHECK(typedEvent1->getKey() == "param");
		CHECK(typedEvent1->getValue() == "some value ");
		evt = reader.getNextEvent();
		REQUIRE(typeid(*evt) == typeid(CommentEvent));
		auto typedEvent2 = dynamic_cast<CommentEvent*>(evt.get());
		CHECK(typedEvent2->getText() == "comment text");
	}

	SECTION("Пустое значение 1")
	{
		std::unique_ptr<std::istream> stream = std::make_unique<std::istringstream>("[section1]\r\nparam1=\r\nparam2=value2");
		Reader reader(std::move(stream));
		auto evt = reader.getNextEvent();
		evt = reader.getNextEvent();
		REQUIRE(typeid(*evt) == typeid(ParameterValueEvent));
		auto typedEvent1 = dynamic_cast<ParameterValueEvent*>(evt.get());
		CHECK(typedEvent1->getKey() == "param1");
		CHECK(typedEvent1->getValue() == "");
	}

	SECTION("Пустое значение 2")
	{
		std::unique_ptr<std::istream> stream = std::make_unique<std::istringstream>("[section1]\r\nparam1=");
		Reader reader(std::move(stream));
		auto evt = reader.getNextEvent();
		evt = reader.getNextEvent();
		REQUIRE(typeid(*evt) == typeid(ParameterValueEvent));
		auto typedEvent1 = dynamic_cast<ParameterValueEvent*>(evt.get());
		CHECK(typedEvent1->getKey() == "param1");
		CHECK(typedEvent1->getValue() == "");
	}
}