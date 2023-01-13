#include <sstream>
#include "Reader.h"
#include "ParseError.h"
#include "CharUtils.h"
#include "CommentEvent.h"
#include "ParameterValueEvent.h"
#include "SectionStartEvent.h"
#include "StringUtils.h"

Reader::Reader(std::shared_ptr<std::istream> is) : m_is(std::move(is))
{
}

std::unique_ptr<ReaderEvent> Reader::getNextEvent()
{
	std::unique_ptr<ReaderEvent> evt;

	while (true)
	{
		char ch;
		auto eof = !m_is->get(ch);
		if (!eof)
		{
			advancePos();
		}

		// Обрабатываем завершения строк
		evt = processEol(eof, ch);
		if (evt)
		{
			return evt;
		}

		if (eof)
		{
			break;
		}

		// Основная логика
		if (CharUtils::isLetter(ch))
		{
			evt = processLetter(ch);
		}
		else if (CharUtils::isNumber(ch))
		{
			evt = processNumber(ch);
		}
		else if (CharUtils::isSpace(ch))
		{
			evt = processSpace(ch);
		}
		else if (ch == CH_SECTION_NAME_START)
		{
			evt = processSectionHeaderStart(ch);
		}
		else if (ch == CH_SECTION_NAME_END)
		{
			evt = processSectionHeaderEnd(ch);
		}
		else if (ch == CH_EQUALITY)
		{
			evt = processEqualitySign(ch);
		}
		else if (ch == CH_COMMENT_START)
		{
			evt = processCommentStart(ch);
		}

		if (evt)
		{
			return evt;
		}

		// для этих событий накапливаем символы в буфер
		if (m_state == COMMENT_STARTED
			|| m_state == SECTION_NAME_STARTED
			|| m_state == WAITING_FOR_SECTION_HEADER_END
			|| m_state == PARAM_NAME_STARTED
			|| m_state == WAITING_FOR_EQUALITY_SIGN
			|| m_state == PARAM_VALUE_STARTED
			)
		{
			m_buf.push_back(ch);
		}
	}

	return std::unique_ptr<ReaderEvent>();
}

void Reader::newLine()
{
	if (m_line == NO_LINE)
	{
		m_line = 1;
	}

	++m_line;
	m_col = NO_COL;
}

void Reader::advancePos()
{
	if (m_line == NO_LINE)
	{
		m_line = 1;
	}

	++m_col;
}

void Reader::saveEventStart()
{
	clearEventBuf();
}

void Reader::raiseParseError(const char* msg) const
{
	throw ParseError(m_line, m_col, msg);
}

void Reader::raiseParseError(const std::string& msg) const
{
	throw ParseError(m_line, m_col, msg);
}

std::string Reader::getEventText() const
{
	return std::string(m_buf.cbegin(), m_buf.cend());
}

void Reader::clearEventBuf()
{
	m_buf.clear();
}

void Reader::updateState(ReaderStates state)
{
	m_state = state;
}

void Reader::updateState(ReaderStates state, bool hasSection)
{
	m_state = state;
	m_hasSection = hasSection;
}

std::unique_ptr<ReaderEvent> Reader::processEol(bool eof, char ch)
{
	std::unique_ptr<ReaderEvent> evt;

	if (eof || CharUtils::isNewLine(ch))
	{
		switch (m_state)
		{
		case COMMENT_STARTED:
		{
			const std::string trimChars = StringUtils::WHITESPACES + CH_COMMENT_START;
			auto commentText = StringUtils::trim(getEventText(), trimChars);
			evt = std::make_unique<CommentEvent>(std::move(commentText));
		}
		break;
		case SECTION_NAME_STARTED:
			raiseParseError("Ожидалось закрытие заголовка секции, а найден перенос строки");
			break;
		case PARAM_NAME_STARTED:
			raiseParseError("Ожидался знак '=', а найден перенос строки");
			break;
		case PARAM_VALUE_STARTED:
		{
			// допускаем значения с пробелами
			std::string trimChars(1, CH_EQUALITY);
			auto paramValue = StringUtils::trim(getEventText(), trimChars);
			evt = std::make_unique<ParameterValueEvent>(std::move(m_paramName), std::move(paramValue));
		}
		break;
		}

		if (!eof && ch == CH_CARET_RETURN && m_state != WAITING_FOR_NEW_LINE_CHAR)
		{
			updateState(WAITING_FOR_NEW_LINE_CHAR);
		}
		else
		{
			updateState(INITIAL);
			newLine();
		}
	}
	else
	{
		// если ждали символа новой строки, а пришло что-то другое, считаем, что началась новая строка в стиле unix
		if (m_state == WAITING_FOR_NEW_LINE_CHAR)
		{
			newLine();
			advancePos();
			updateState(INITIAL);
		}
	}

	return evt;
}

std::unique_ptr<ReaderEvent> Reader::processLetter(char ch)
{
	switch (m_state)
	{
	case INITIAL:
		if (!m_hasSection)
		{
			raiseParseError("Идентификаторы параметров не допускаются вне секций");
		}
		else
		{
			updateState(PARAM_NAME_STARTED);
			saveEventStart();
		}
		break;
	case WAITING_FOR_SECTION_NAME:
		updateState(SECTION_NAME_STARTED);
		saveEventStart();
		break;
	case WAITING_FOR_EQUALITY_SIGN:
	{
		std::stringstream s;
		s << "Ожидался знак '=', а найден символ '" << ch << "'";
		raiseParseError(s.str());
		break;
	}
	case WAITING_FOR_SECTION_HEADER_END:
		updateState(SECTION_NAME_STARTED);
		break;
	}

	return std::unique_ptr<ReaderEvent>();
}

std::unique_ptr<ReaderEvent> Reader::processNumber(char ch)
{
	// на данном этапе не делаем различий между буквами и цифрами. Считаем, что идентификатор секции или параметра может начинаться с цифры
	return processLetter(ch);
}

std::unique_ptr<ReaderEvent> Reader::processSpace(char ch)
{
	switch (m_state)
	{
	case PARAM_NAME_STARTED:
		updateState(WAITING_FOR_EQUALITY_SIGN);
		break;
	case SECTION_NAME_STARTED:
		updateState(WAITING_FOR_SECTION_HEADER_END);
		break;
	}

	return std::unique_ptr<ReaderEvent>();
}

std::unique_ptr<ReaderEvent> Reader::processSectionHeaderStart(char ch)
{
	switch (m_state)
	{
	case INITIAL:
		m_state = WAITING_FOR_SECTION_NAME;
		break;
	case WAITING_FOR_SECTION_NAME:
	case WAITING_FOR_SECTION_HEADER_END:
	case SECTION_NAME_STARTED:
		raiseParseError("Обнаружено открытие новой секции, когда предыдущая еще не закрыта");
		break;
	case PARAM_NAME_STARTED:
	{
		std::stringstream s;
		s << "Ожидался знак '=', а найден символ '" << ch << "'";
		raiseParseError(s.str());
	}
	break;
	case PARAM_VALUE_STARTED:
		raiseParseError("Секция дожна начинаться на новой строке");
		break;
	}

	return std::unique_ptr<ReaderEvent>();
}

std::unique_ptr<ReaderEvent> Reader::processSectionHeaderEnd(char ch)
{
	switch (m_state)
	{
	case INITIAL:
	case WAITING_FOR_SECTION_NAME:
	{
		std::stringstream s;
		s << "Ожидалось имя секции, а найден символ '" << ch << "'";
		raiseParseError(s.str());
	}
	break;
	case SECTION_NAME_STARTED:
	case WAITING_FOR_SECTION_HEADER_END:
		updateState(WAITING_FOR_LINE_END, true);
		{
			auto sectionName = StringUtils::trim(getEventText());
			return std::make_unique<SectionStartEvent>(std::move(sectionName));
		}
		break;
	case PARAM_NAME_STARTED:
	{
		std::stringstream s;
		s << "Ожидался знак '=', а найден символ '" << ch << "'";
		raiseParseError(s.str());
	}
	break;
	case PARAM_VALUE_STARTED:
	{
		std::stringstream s;
		s << "Ожидалось значение параметра, а найден символ '" << ch << "'";
		raiseParseError(s.str());
	}
	break;
	}

	return std::unique_ptr<ReaderEvent>();
}

std::unique_ptr<ReaderEvent> Reader::processEqualitySign(char ch)
{
	switch (m_state)
	{
	case INITIAL:
	{
		std::stringstream s;
		s << "Ожидалось имя параметра, а найден символ '" << ch << "'";
		raiseParseError(s.str());
	}
	break;
	case SECTION_NAME_STARTED:
	{
		std::stringstream s;
		s << "Ожидалось завершение заголовка секции, а найден символ '" << ch << "'";
		raiseParseError(s.str());
	}
	break;
	case WAITING_FOR_EQUALITY_SIGN:
	case PARAM_NAME_STARTED:
		m_paramName = StringUtils::trim(getEventText());
		updateState(PARAM_VALUE_STARTED);
		clearEventBuf();
		break;
	case PARAM_VALUE_STARTED:
	{
		std::stringstream s;
		s << "Ожидалось значение параметра, а найден символ '" << ch << "'";
		raiseParseError(s.str());
	}
	break;
	}

	return std::unique_ptr<ReaderEvent>();
}

std::unique_ptr<ReaderEvent> Reader::processCommentStart(char ch)
{
	switch (m_state)
	{
	case PARAM_VALUE_STARTED:
	{
		std::string trimChars(1, CH_EQUALITY);
		auto paramValue = StringUtils::trim(getEventText(), trimChars);
		updateState(COMMENT_STARTED);
		saveEventStart();
		return std::make_unique<ParameterValueEvent>(std::move(m_paramName), std::move(paramValue));
	}
	break;
	case PARAM_NAME_STARTED:
	{
		std::stringstream s;
		s << "Ожидался знак '=', а найден символ '" << ch << "'";
		raiseParseError(s.str());
	}
	break;
	case SECTION_NAME_STARTED:
	{
		std::stringstream s;
		s << "Ожидалось завершение заголовка секции, а найден символ '" << ch << "'";
		raiseParseError(s.str());
	}
	break;
	default:
		updateState(COMMENT_STARTED);
		saveEventStart();
		break;
	}

	return std::unique_ptr<ReaderEvent>();
}
