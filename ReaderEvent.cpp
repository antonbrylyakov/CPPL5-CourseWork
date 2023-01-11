#include "ReaderEvent.h"

ReaderEvent::ReaderEvent(size_t line, size_t col) : m_line(line), m_col(col)
{
}

ReaderEvent::~ReaderEvent()
{
}

size_t ReaderEvent::getLine()
{
	return size_t();
}

size_t ReaderEvent::getCol()
{
	return size_t();
}

std::ostream& operator<<(std::ostream& os, const ReaderEvent& evt)
{
	evt.print(os);
	return os;
}
