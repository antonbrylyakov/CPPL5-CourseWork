#include "ParameterValueEvent.h"

ParameterValueEvent::ParameterValueEvent(std::string&& key, std::string&& value)
	: ReaderEvent(), m_key(key), m_value(value)
{
}

const std::string& ParameterValueEvent::getKey() const
{
	return m_key;
}

const std::string& ParameterValueEvent::getValue() const
{
	return m_value;
}

void ParameterValueEvent::print(std::ostream& os) const
{
	os << "Параметр: '" << m_key << "; Значение: '" << m_value << "'";
}
