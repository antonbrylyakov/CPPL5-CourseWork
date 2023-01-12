#pragma once
#include "ReaderEvent.h"
#include <string>


class ParameterValueEvent : public ReaderEvent
{
public:
	ParameterValueEvent(std::string&& key, std::string&& value);
	const std::string& getKey() const;
	const std::string& getValue() const;
protected:
	void print(std::ostream& os) const override;
private:
	std::string m_key;
	std::string m_value;
};