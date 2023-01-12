#include <iostream>
#include <catch2/catch_session.hpp>
#ifdef _WIN32
#include "windows.h"
#endif


int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");
#ifdef _WIN32
	SetConsoleCP(1251);
#endif

	return Catch::Session().run(argc, argv);
}