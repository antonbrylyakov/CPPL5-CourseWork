#include "ErrorEvent.h"

ErrorEvent::ErrorEvent(size_t line, size_t col, std::string&& message) : ReaderEvent(line, col), m_message(message)
{
}

void ErrorEvent::print(std::ostream& os) const
{
	os << m_message;
}
