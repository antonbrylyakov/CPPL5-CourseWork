cmake_minimum_required(VERSION 3.2.1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
project(IniFileParserApp)
add_subdirectory(IniFileParser)
add_subdirectory(IniFileParserApp)