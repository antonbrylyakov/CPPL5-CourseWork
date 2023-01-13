#include <iostream>
#ifdef _WIN32
#include "windows.h"
#endif
#include "IniParser.h"


int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");
#ifdef _WIN32
	SetConsoleCP(1251);
#endif

	IniParser parser("example.ini");
	try
	{
		std::cout << parser.getValue<int>("Section4.Mode") << std::endl;
	}
	catch (std::runtime_error& e)
	{
		std::cout << "Ошибка обработки ini-файла:" << std::endl << e.what() << std::endl;
	}
}