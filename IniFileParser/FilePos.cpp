#include "FilePos.h"


void FilePos::newLine()
{
	++m_line;
	m_col = NO_COL;
}

void FilePos::advance()
{
	if (m_line == NO_LINE)
	{
		m_line = 1;
	}

	++m_col;
}

size_t FilePos::getLine()
{
	return m_line;
}
size_t FilePos::getCol()
{
	return m_col;
}
