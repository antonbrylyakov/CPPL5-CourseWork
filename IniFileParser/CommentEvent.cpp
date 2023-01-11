#include "CommentEvent.h"

CommentEvent::CommentEvent(size_t line, size_t col, std::string&& text) : ReaderEvent(line, col), m_text(text)
{
}

const std::string& CommentEvent::getText() const
{
	return m_text;
}

void CommentEvent::print(std::ostream& os) const
{
	os << "Комментарий: '" << m_text << "'";
}
