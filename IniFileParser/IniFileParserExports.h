#pragma once

#ifdef _WIN32
#ifdef  INIFILEPARSER_EXPORTS 
#define INIFILEPARSER_API __declspec(dllexport)
#else
#ifndef INIFILEPARSER_TESTS
#define INIFILEPARSER_API __declspec(dllimport)
#else
#define INIFILEPARSER_API
#endif
#endif
#else
#define INIFILEPARSER_API
#endif