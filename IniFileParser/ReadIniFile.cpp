#include <string>
#include <list>
#include <sstream>
#include "ReadIniFile.h"
#include "Constants.h"
#include "CommentEvent.h"
#include "ParseError.h"
#include "CharUtils.h"
#include "FilePos.h"
#include "SectionStartEvent.h"
#include "ParameterValueEvent.h"


void readIniFile(std::istream& is, void(*evtCallback)(const ReaderEvent& evt))
{
	ReaderStates state = INITIAL;
	FilePos currentPos, eventStartPos;
	char ch;
	std::list<char> eventBuf;
	std::string paramName;

	auto raiseError = [evtCallback, &currentPos](std::string&& msg)
	{
		throw ParseError(msg);
	};

	while (true)
	{
		auto eof = !is.get(ch);

		if (!eof)
		{
			currentPos.advance();
		}

		// обработка конца строки

		if (eof || ch == CH_CARET_RETURN || ch == CH_NEW_LINE)
		{
			switch (state)
			{
			case COMMENT_STARTED:
			{
				std::string commentText(eventBuf.cbegin(), eventBuf.cend());
				CommentEvent evt(currentPos.getLine(), currentPos.getCol(), std::move(commentText));
				evtCallback(evt);
			}
			break;
			case SECTION_NAME_STARTED:
				raiseError("Ожидалось закрытие заголовка секции, а найден перенос строки");
				break;
			case PARAM_NAME_STARTED:
				raiseError("Ожидался знак '=', а найден перенос строки");
				break;
			case PARAM_VALUE_STARTED:
			{
				std::string paramValue(eventBuf.cbegin(), eventBuf.cend());
				ParameterValueEvent evt(currentPos.getLine(), currentPos.getCol(), std::move(paramName), std::move(paramValue));
				evtCallback(evt);
			}
			break;
			}

			if (ch == CH_CARET_RETURN && state != WAITING_FOR_NEW_LINE_CHAR)
			{
				state = WAITING_FOR_NEW_LINE_CHAR;
			}
			else
			{
				state = INITIAL;
				currentPos.newLine();
			}
		}
		else
		{
			// если ждали символа новой строки, а пришло что-то другое, считаем, что началась новая строка в стиле unix
			if (state == WAITING_FOR_NEW_LINE_CHAR)
			{
				currentPos.newLine();
				currentPos.advance();
			}
		}

		if (eof)
		{
			break;
		}

		// основная логика

		if (CharUtils::isLetter(ch))
		{
			switch (state)
			{
			case INITIAL:
				raiseError("Идентификаторы параметров не допускаются вне секций");
				break;
			case WAITING_FOR_SECTION_NAME:
				state = SECTION_NAME_STARTED;
				eventBuf.clear();
				eventStartPos = currentPos;
				break;
			case SECTION_BODY_STARTED:
				eventBuf.clear();
				eventStartPos = currentPos;
				break;
			case WAITING_FOR_EQUALITY_SIGN:
			{
				std::stringstream s;
				s << "Ожидался знак '=', а найден символ '" << ch << "'";
				raiseError(s.str());
			}
			break;
			}
		}
		else if (CharUtils::isNumber(ch))
		{
			switch (state)
			{
			case INITIAL:
			{
				std::stringstream s;
				s << "Ожидалось начало секции, а найден символ '" << ch << "'";
				raiseError(s.str());
			}
			break;
			case WAITING_FOR_SECTION_NAME:
				raiseError("Название секции не может начинаться с цифры");
				break;
			case SECTION_BODY_STARTED:
				raiseError("Название параметра не может начинаться с цифры");
				break;
			case WAITING_FOR_EQUALITY_SIGN:
			{
				std::stringstream s;
				s << "Ожидался знак '=', а найден символ '" << ch << "'";
				raiseError(s.str());
			}
			break;
			}
		}
		else if (CharUtils::isSpace(ch))
		{
			switch (state)
			{
			case PARAM_NAME_STARTED:
				state = WAITING_FOR_EQUALITY_SIGN;
				break;
			}
		}
		else if (ch == CH_SECTION_NAME_START)
		{
			switch (state)
			{
			case INITIAL:
			case SECTION_BODY_STARTED:
				state = WAITING_FOR_SECTION_NAME;
				break;
			case WAITING_FOR_SECTION_NAME:
			case SECTION_NAME_STARTED:
				raiseError("Обнаружено открытие новой секции, когда предыдущая еще не закрыта");
				break;
			case PARAM_NAME_STARTED:
			{
				std::stringstream s;
				s << "Ожидался знак '=', а найден символ '" << ch << "'";
				raiseError(s.str());
			}
			break;
			case PARAM_VALUE_STARTED:
				raiseError("Секция дожна начинаться на новой строке");
				break;
			}
		}
		else if (ch == CH_SECTION_NAME_END)
		{
			switch (state)
			{
			case INITIAL:
			case WAITING_FOR_SECTION_NAME:
			{
				std::stringstream s;
				s << "Ожидалось имя секции, а найден символ '" << ch << "'";
				raiseError(s.str());
			}
			break;
			case SECTION_BODY_STARTED:
				raiseError("Закрытие заголовка секции без соответствующего открытия");
				break;
			case SECTION_NAME_STARTED:
				state = WAITING_FOR_LINE_END;
				{
					std::string sectionName(eventBuf.cbegin(), eventBuf.cend());
					SectionStartEvent evt(currentPos.getLine(), currentPos.getCol(), std::move(sectionName));
				}
				break;
			case PARAM_NAME_STARTED:
			{
				std::stringstream s;
				s << "Ожидался знак '=', а найден символ '" << ch << "'";
				raiseError(s.str());
			}
			break;
			case PARAM_VALUE_STARTED:
			{
				std::stringstream s;
				s << "Ожидалось значение параметра, а найден символ '" << ch << "'";
				raiseError(s.str());
			}
			break;
			}
		}
		else if (ch == CH_EQUALITY)
		{
			switch (state)
			{
			case INITIAL:
			case SECTION_BODY_STARTED:
			{
				std::stringstream s;
				s << "Ожидалось имя параметра, а найден символ '" << ch << "'";
				raiseError(s.str());
			}
			break;
			case SECTION_NAME_STARTED:
			{
				std::stringstream s;
				s << "Ожидалось завершение заголовка секции, а найден символ '" << ch << "'";
				raiseError(s.str());
			}
			break;
			case PARAM_NAME_STARTED:
				paramName = std::string(eventBuf.cbegin(), eventBuf.cend());
				state = PARAM_VALUE_STARTED;
				eventBuf.clear();
				break;
			case PARAM_VALUE_STARTED:
			{
				std::stringstream s;
				s << "Ожидалось значение параметра, а найден символ '" << ch << "'";
				raiseError(s.str());
			}
			break;
			}
		}

		// для этих событий накапливаем символы в буфер
		if (state == COMMENT_STARTED || state == PARAM_NAME_STARTED || state == SECTION_NAME_STARTED || state == PARAM_VALUE_STARTED)
		{
			eventBuf.push_back(ch);
		}
	}
}


