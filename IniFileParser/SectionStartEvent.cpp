#include "SectionStartEvent.h"

SectionStartEvent::SectionStartEvent(std::string&& sectionName) : ReaderEvent(), m_sectionName(sectionName)
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
