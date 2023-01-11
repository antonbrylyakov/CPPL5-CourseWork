#pragma once
#include "ReaderEvent.h"
#include <string>


class SectionStartEvent : public ReaderEvent
{
public:
	SectionStartEvent(size_t line, size_t col, std::string&& sectionName);
	const std::string& getSectionName() const;
protected:
	void print(std::ostream& os) const override;
private:
	std::string m_sectionName;
};