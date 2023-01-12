#include "CommentEvent.h"

CommentEvent::CommentEvent( std::string&& text) : ReaderEvent(), m_text(text)
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
