#pragma once
#include "ReaderEvent.h"
#include <string>


class CommentEvent : public ReaderEvent
{
public:
	CommentEvent(std::string&& text);
	const std::string& getText() const;
protected:
	void print(std::ostream& os) const override;
private:
	std::string m_text;
};