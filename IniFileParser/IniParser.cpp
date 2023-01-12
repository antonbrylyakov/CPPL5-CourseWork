#include "IniParser.h"
#include "ParameterValueEvent.h"
#include "SectionStartEvent.h"
#include "Reader.h"
#include <fstream>
#include <memory>
#include <algorithm>

IniParser::IniParser(const std::string& fileName): m_rawValues(), m_reader(std::make_unique<std::ifstream>(fileName))
{
}

IniParser::IniParser(std::unique_ptr<std::istream> is): m_rawValues(), m_reader(std::move(is))
{	
}

/*/std::string IniParser::getRawValue(const std::string& path)
{

}*/

void IniParser::initialize()
{
	std::string currentSectionName;
	while (auto evt = m_reader.getNextEvent())
	{
		if (typeid(*evt) == typeid(SectionStartEvent))
		{
			auto sectionStartEvent = dynamic_cast<SectionStartEvent*>(evt.get());
			std::transform(sectionStartEvent->getSectionName().begin(),
				sectionStartEvent->getSectionName().end(),
				currentSectionName.begin(),
				::tolower);
		}
		else if (typeid(*evt) == typeid(ParameterValueEvent))
		{
			auto parameterValueEvent = dynamic_cast<ParameterValueEvent*>(evt.get());
			m_rawValues[currentSectionName][parameterValueEvent->getKey()] = parameterValueEvent->getValue();
		}
	}
}