cmake_minimum_required(VERSION 3.2.1)
project(IniFileParserApp)
set(sources  "main.cpp")
add_executable(IniFileParserApp ${sources})
target_link_libraries(IniFileParserApp IniFileParser)
target_include_directories(IniFileParserApp PRIVATE "../IniFileParser")
# Копируем файл с примером
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/example.ini ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/example.ini COPYONLY)