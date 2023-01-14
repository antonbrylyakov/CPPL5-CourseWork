#pragma once
#include <stdexcept>
#include "IniFileParserExports.h"

// Исключение, возникающее при некорректной операции чтения параметра
class INIFILEPARSER_API ReadParamError : public std::runtime_error
{
public:
	ReadParamError(const std::string& msg) : std::runtime_error(msg.c_str()) {}

	ReadParamError(const char* msg) : std::runtime_error(msg) {}
};
