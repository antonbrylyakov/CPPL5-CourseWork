#include "CharUtils.h"

namespace CharUtils
{
	bool isLetter(char ch)
	{
		return ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z';
	}

	bool isNumber(char ch)
	{
		return ch >= '0' && ch <= '9';
	}
	bool isSpace(char ch)
	{
		return ch == ' ' || ch == '\t';
	}
	bool isNewLine(char ch)
	{
		return ch == '\r' || ch == '\n';
	}
}