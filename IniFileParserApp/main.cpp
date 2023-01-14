#include <iostream>
#ifdef _WIN32
#include "windows.h"
#endif
#include "IniParser.h"
#include "ParseError.h"
#include "ReadParamError.h"


int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");
#ifdef _WIN32
	SetConsoleCP(1251);
#endif

	IniParser parser("example.ini");
	try
	{
		std::cout << parser.getValue<std::string>("section2.Var8") << std::endl;
	}
	catch (ReadParamError& e)
	{
		std::cout << "Ошибка обработки ini-файла:" << std::endl << e.what() << std::endl;
	}
	catch (ParseError& e)
	{
		std::cout << "Входной ini-файла содержит неверный синтаксис:" << std::endl 
			<< "Строка: " << e.getLine() << " : " << e.what() << std::endl;
	}
}