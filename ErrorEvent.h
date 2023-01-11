#pragma once
#include "ReaderEvent.h"

class ErrorEvent : public ReaderEvent
{
public:
	ErrorEvent(size_t line, size_t col, std::string&& message);
protected:
	void print(std::ostream& os) const override;
private:
	std::string m_message;
};