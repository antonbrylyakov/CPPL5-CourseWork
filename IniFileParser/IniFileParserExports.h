#pragma once

#ifdef _WIN32
#ifdef  INIFILEPARSER_EXPORTS 
#define INIFILEPARSER_API __declspec(dllexport)
#else
#define INIFILEPARSER_API __declspec(dllimport)
#endif
#else
#define INIFILEPARSER_API
#endif