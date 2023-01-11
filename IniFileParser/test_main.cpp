#include <iostream>
#ifdef _WIN32
#include "windows.h"
#endif


int main()
{
	setlocale(LC_ALL, "Russian");
#ifdef _WIN32
	SetConsoleCP(1251);
#endif




}