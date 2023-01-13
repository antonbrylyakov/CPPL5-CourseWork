#pragma once
#include <stdexcept>

class ReadParamError : public std::runtime_error
{
public:
	ReadParamError(const std::string& msg) : std::runtime_error(msg.c_str()) {}

	ReadParamError(const char* msg) : std::runtime_error(msg) {}
};
