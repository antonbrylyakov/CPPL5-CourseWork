#pragma once
#include <istream>
#include <memory>
#include <list>
#include "ReaderEvent.h"


// Класс для чтения входного ini-файла из потока
class Reader 
{
public:
	// Используем входной параметр как shared_ptr, 
	// т.к. для потоков разных типов может потребоваться управление извне.
	// Например, закрытие файлового потока
	Reader(std::shared_ptr<std::istream> is);
	// Метод получения очередного события, возникающего по мере чтения файла
	std::unique_ptr<ReaderEvent> getNextEvent();

private:
	// состояние автомата
	ReaderStates m_state = INITIAL;
	bool m_hasSection = false;

	size_t m_line = NO_LINE;
	size_t m_col = NO_COL;
	std::list<char> m_buf;
	std::shared_ptr<std::istream> m_is;
	std::string m_paramName;
	void newLine();
	void advancePos();
	void saveEventStart();
	void raiseParseError(const char* msg) const;
	void raiseParseError(const std::string& msg) const;
	std::string getEventText() const;
	void clearEventBuf();
	void updateState(ReaderStates state);
	void updateState(ReaderStates state, bool hasSection);
	std::unique_ptr<ReaderEvent> processEol(bool eof, char ch);
	std::unique_ptr<ReaderEvent> processLetter(char ch);
	std::unique_ptr<ReaderEvent> processNumber(char ch);
	std::unique_ptr<ReaderEvent> processSpace(char ch);
	std::unique_ptr<ReaderEvent> processSectionHeaderStart(char ch);
	std::unique_ptr<ReaderEvent> processSectionHeaderEnd(char ch);
	std::unique_ptr<ReaderEvent> processEqualitySign(char ch);
	std::unique_ptr<ReaderEvent> processCommentStart(char ch);
};