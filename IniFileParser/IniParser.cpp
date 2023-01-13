#include <fstream>
#include <memory>
#include <vector>
#include <sstream>
#include "IniParser.h"
#include "ParameterValueEvent.h"
#include "SectionStartEvent.h"
#include "Reader.h"
#include "StringUtils.h"
#include "ReadParamError.h"
#include "MiscUtils.h"

IniParser::IniParser(const std::string& fileName): m_rawValues(), m_fileName(fileName)
{
}

IniParser::IniParser(std::shared_ptr<std::istream> is): m_rawValues(), m_is(is)
{	
}

std::string IniParser::getRawValue(const std::string& path)
{
	if (!m_initialized)
	{
		initialize();
	}

	std::vector<std::string> pathSegments;
	StringUtils::split(path, pathSegments, '.');

	if (pathSegments.size() == 2)
	{
		const auto sectionName = pathSegments[0];
		const auto paramName = pathSegments[1];
		const auto sectionData = m_rawValues.find(sectionName);
		if (sectionData != m_rawValues.end())
		{
			const auto paramData = sectionData->second.find(paramName);
			if (paramData != sectionData->second.end())
			{
				return paramData->second;
			}
			else
			{
				std::stringstream s;
				s << "Неверное имя параметра " << paramName << " в секции " << sectionName << ". Допустимые параметры: ";
				MiscUtils::printKeys(sectionData->second, s);
				throw ReadParamError(s.str());
			}
		}
		else
		{
			std::stringstream s;
			s << "Неверное имя секции '" << sectionName << "'" << ". Допустимые секции: ";
			MiscUtils::printKeys(m_rawValues, s);
			throw ReadParamError(s.str());
		}
	}
	else
	{
		std::stringstream s;
		s << "Переданный путь к параметру не является валидным '" << path << "'";
		throw ReadParamError(s.str());
	}
}

void IniParser::initializeFromStream(std::shared_ptr<std::istream> is)
{
	std::string currentSectionName;
	Reader reader(is);
	while (auto evt = reader.getNextEvent())
	{
		if (auto sectionStartEvent = dynamic_cast<SectionStartEvent*>(evt.get()))
		{
			currentSectionName = StringUtils::toLower(sectionStartEvent->getSectionName());
		}
		else if (typeid(*evt) == typeid(ParameterValueEvent))
		{
			auto parameterValueEvent = dynamic_cast<ParameterValueEvent*>(evt.get());
			m_rawValues[currentSectionName][parameterValueEvent->getKey()] = parameterValueEvent->getValue();
		}
	}

	m_initialized = true;
}

void IniParser::initialize()
{
	if (m_is)
	{
		initializeFromStream(m_is);
	}
	else
	{
		// вариант загрузки из файла
		auto ifs = std::make_shared<std::ifstream>(m_fileName);
		initializeFromStream(ifs);
		ifs->close();
	}
}