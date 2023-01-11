#include "FilePos.h"


void FilePos::newLine()
{
	++m_line;
	m_col = NO_COL;
}

void FilePos::advance()
{
	++m_col;
}

void FilePos::initIfNew()
{
	if (m_line == NO_LINE)
	{
		m_line = 1;
	}
}

size_t FilePos::getLine()
{
	return m_line;
}
size_t FilePos::getCol()
{
	return m_col;
}
