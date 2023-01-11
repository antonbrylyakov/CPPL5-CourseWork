#include <string>
#include <list>
#include "ReadIniFile.h"
#include "Constants.h"
#include "ReadFinishEvent.h"
#include "CommentEvent.h"
#include "CharUtils.h"
#include "FilePos.h"


void ReadIniFile(std::istream& is, void(*evtCallback)(const ReaderEvent& evt))
{
	ReaderStates state = INITIAL;
	FilePos currentPos, eventStartPos;
	char ch;
	std::list<char> eventBuf;

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
				// raise comment event
				break;
			case SECTION_NAME_STARTED:
				// raise error that section name not closed
				break;
			case PARAM_NAME_STARTED:
				// raise error that param value is missing
				break;
			case PARAM_VALUE_STARTED:
				// raise param event
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
				// raise error that section is not started
				break;
			case SECTION_BODY_STARTED:
				eventBuf.clear();
				eventStartPos = currentPos;
				break;
			}
		}
		else if (CharUtils::isNumber(ch))
		{
			switch (state)
			{
			case INITIAL:
				// raise error that section is not started
				break;
			case SECTION_BODY_STARTED:
				// raise error that param name cannot start from number
				break;
			}
		}
		else if (ch == CH_SECTION_NAME_START)
		{
			switch (state)
			{
			case INITIAL:
			case SECTION_BODY_STARTED:
				state = SECTION_NAME_STARTED;
				eventBuf.clear();
				eventStartPos = currentPos;
				break;
			case SECTION_NAME_STARTED:
				// raise error that section name already started
				break;
			case PARAM_NAME_STARTED:
				// raise error that param value is missing
				break;
			case PARAM_VALUE_STARTED:
				// raise error that section should start on a new line
				break;
			}
		}
		else if (ch == CH_SECTION_NAME_END)
		{
			switch (state)
			{
			case INITIAL:
			case SECTION_BODY_STARTED:
				// raise error that section name is not started
				break;
			case SECTION_NAME_STARTED:
				// raise section start event
				break;
			case PARAM_NAME_STARTED:
				// raise error that param value is missing
				break;
			case PARAM_VALUE_STARTED:
				// raise error that not expected end of section
				break;
			}
		}
		else if (ch == CH_EQUALITY)
		{
			switch (state)
			{
			case INITIAL:
			case SECTION_BODY_STARTED:
				// raise error that section name is not started
				break;
			case SECTION_NAME_STARTED:
				// raise section start event
				break;
			case PARAM_NAME_STARTED:
				
				break;
			case PARAM_VALUE_STARTED:
				// raise error that not expected end of section
				break;
			}
		}

		// для этих событий накапливаем символы в буфер
		if (state == COMMENT_STARTED || state == PARAM_NAME_STARTED || state == SECTION_NAME_STARTED || state == PARAM_VALUE_STARTED)
		{
			eventBuf.push_back(ch);
		}
	}

	const ReadFinishEvent evt(currentPos.getLine(), currentPos.getCol());
	evtCallback(evt);
}


