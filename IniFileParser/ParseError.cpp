#include "ParseError.h"

size_t ParseError::getLine() const
{
	return m_line;
}

size_t ParseError::getCol() const
{
	return m_col;
}
