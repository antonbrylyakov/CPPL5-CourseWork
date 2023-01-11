#pragma once
#include "Constants.h"
#include <ostream>

class ReaderEvent
{
public:
	ReaderEvent(size_t line, size_t col);
	virtual ~ReaderEvent();
	size_t getLine();
	size_t getCol();
	friend std::ostream& operator<<(std::ostream& os, const ReaderEvent& evt);
protected:
	virtual void print(std::ostream& os) const = 0;
private:
	size_t m_line = NO_LINE;
	size_t m_col = NO_COL;
};