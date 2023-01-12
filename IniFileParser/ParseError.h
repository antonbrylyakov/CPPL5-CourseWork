#pragma once
#include <stdexcept>
#include <string>



class ParseError : public std::runtime_error
{
public:
	ParseError(size_t line, size_t col, const std::string& msg) : m_line(line), m_col(col), runtime_error(msg.c_str()) {}

	ParseError(size_t line, size_t col, const char* msg) : m_line(line), m_col(col), runtime_error(msg) {}
private:
	size_t m_line = NO_LINE;
	size_t m_col = NO_COL;
};