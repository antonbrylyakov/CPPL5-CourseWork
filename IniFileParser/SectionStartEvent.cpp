#include "SectionStartEvent.h"

SectionStartEvent::SectionStartEvent(size_t line, size_t col, std::string&& sectionName) : ReaderEvent(line, col), m_sectionName(sectionName)
{
}

const std::string& SectionStartEvent::getSectionName() const
{
	return m_sectionName;
}

void SectionStartEvent::print(std::ostream& os) const
{
	os << "Начало секции: '" << m_sectionName << "'";
}
