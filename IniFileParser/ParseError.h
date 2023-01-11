#pragma once
#include <stdexcept>
#include <string>



class ParseError : public std::runtime_error
{
public:
	explicit ParseError(const std::string& msg) : runtime_error(msg.c_str()) {}

	explicit ParseError(const char* msg) : runtime_error(msg) {}
};