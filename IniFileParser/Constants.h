#pragma once
static const size_t NO_LINE = 0;
static const size_t NO_COL = 0;

static const char CH_EQUALITY = '=';
static const char CH_SECTION_NAME_START = '[';
static const char CH_SECTION_NAME_END = ']';
static const char CH_COMMENT_START = ';';
static const char CH_CARET_RETURN = '\r';
static const char CH_NEW_LINE = '\n';


enum ReaderStates
{
	INITIAL = 0,
	COMMENT_STARTED,
	WAITING_FOR_SECTION_NAME,
	SECTION_NAME_STARTED,
	WAITING_FOR_SECTION_HEADER_END,
	SECTION_BODY_STARTED,
	PARAM_NAME_STARTED,
	WAITING_FOR_EQUALITY_SIGN,
	PARAM_VALUE_STARTED,
	WAITING_FOR_LINE_END,
	WAITING_FOR_NEW_LINE_CHAR
};
