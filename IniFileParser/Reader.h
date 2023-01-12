#pragma once
#include <istream>
#include <memory>
#include <list>
#include "ReaderEvent.h"



class Reader 
{
public:
	Reader(std::unique_ptr<std::istream> is);
	std::unique_ptr<ReaderEvent> getNextEvent();

private:
	ReaderStates m_state = INITIAL;
	size_t m_line = NO_LINE;
	size_t m_col = NO_COL;
	size_t m_eventStartLine = NO_LINE;
	size_t m_eventStartCol = NO_COL;
	std::list<char> m_buf;
	std::unique_ptr<std::istream> m_is;
	std::string m_paramName;
	void newLine();
	void advancePos();
	void saveEventStart();
	void raiseParseError(const char* msg) const;
	void raiseParseError(const std::string& msg) const;
	std::string getEventText() const;
	void clearEventBuf();
	std::unique_ptr<ReaderEvent> processEol(bool eof, char ch);
	std::unique_ptr<ReaderEvent> processLetter(char ch);
	std::unique_ptr<ReaderEvent> processNumber(char ch);
	std::unique_ptr<ReaderEvent> processSpace(char ch);
	std::unique_ptr<ReaderEvent> processSectionHeaderStart(char ch);
	std::unique_ptr<ReaderEvent> processSectionHeaderEnd(char ch);
	std::unique_ptr<ReaderEvent> processEqualitySign(char ch);
	std::unique_ptr<ReaderEvent> processCommentStart(char ch);
};