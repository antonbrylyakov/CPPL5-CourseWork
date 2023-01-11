#pragma once
#include "Constants.h"

class FilePos
{
public:
	void newLine();
	void advance();
	size_t getLine();
	size_t getCol();

private:
	size_t m_line = NO_LINE;
	size_t m_col = NO_COL;
};