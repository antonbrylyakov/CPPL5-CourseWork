#include <iostream>
#ifdef _WIN32
#include "windows.h"
#endif
#include "IniParser.h"
#include "ParseError.h"
#include "ReadParamError.h"

template<typename T>
void tryGetParamAndPrint(IniParser& parser, const std::string& name)
{
	try
	{
		std::cout << "Попытка чтения параметра '" << name << "'..." << std::endl;
		std::cout << parser.getValue<T>(name) << std::endl;
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

	std::cout << std::endl;
}

// Пример использования парсера
int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");
#ifdef _WIN32
	SetConsoleCP(1251);
#endif

	std::string fileName("exaple.ini");
	IniParser parser1(fileName);
	std::cout << "Чтение файла '" << fileName << "'" << std::endl;
	tryGetParamAndPrint<std::string>(parser1, "Section1.var1");
	tryGetParamAndPrint<std::string>(parser1, "Section1.var2");
	tryGetParamAndPrint<std::string>(parser1, "Section18.var1");
	tryGetParamAndPrint<std::string>(parser1, "Section1.var8");
	tryGetParamAndPrint<std::string>(parser1, "Section1.var3");

	fileName = "example_error.ini";
	IniParser parser2(fileName);
	std::cout << "Чтение файла '" << fileName << "'" << std::endl;
	tryGetParamAndPrint<std::string>(parser2, "Section1.var1");
}