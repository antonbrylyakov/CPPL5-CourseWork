#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include "Reader.h"
#include "CommentEvent.h"
#include "SectionStartEvent.h"
#include "ParseError.h"

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